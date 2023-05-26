/*
 * @author: BL-GS 
 * @date:   2023/5/26
 */

#pragma once
#ifndef ALGORITHM_THREAD_NUMA_BIND_THREAD_H
#define ALGORITHM_THREAD_NUMA_BIND_THREAD_H

#include <thread>

#include <memory/memory.h>
#include <logger/logger.h>

namespace algorithm::thread {

	class NUMABindThread {
	public:
		static constexpr int INVALID_ID = -1;

	private:
		int cpu_id_  = INVALID_ID;

		int numa_id_ = INVALID_ID;

		std::thread inner_thread_;

	public:
		template<class Func>
		NUMABindThread(int numa_id, Func &&func): inner_thread_(
            [numa_id, inner_func = std::forward<Func>(func), this] {
                inner_thread_func(numa_id, inner_func);
            }) {}

		template<class Func>
		NUMABindThread(int tid, int numa_id, Func &&func): inner_thread_(
           [tid, numa_id, inner_func = std::forward<Func>(func), this] {
               inner_thread_func(tid, numa_id, inner_func);
           }) {}

	private:
		void inner_thread_preload(int numa_id) {
			memory::THREAD_CONTEXT.allocate_tid();
			cpu_id_ = memory::THREAD_CONTEXT.bind_cpu_on_node(numa_id);
			numa_id_ = numa_id;
		}

		void inner_thread_preload(int tid, int numa_id) {
			if (!memory::THREAD_CONTEXT.bind_tid(tid)) {
				util::logger::logger_error("Unable to bind tid ", tid);
			}
			cpu_id_ = memory::THREAD_CONTEXT.bind_cpu_on_node(numa_id);
			numa_id_ = numa_id;
		}

		template<class Func>
		static void inner_thread_body(Func &func) {
			func();
		}

		static void inner_thread_last() {}

		template<class Func>
		void inner_thread_func(int numa_id, Func &func) {
			inner_thread_preload(numa_id);
			inner_thread_body(std::forward<Func>(func));
			inner_thread_last();
		}

		template<class Func>
		void inner_thread_func(int tid, int numa_id, Func &func) {
			inner_thread_preload(tid, numa_id);
			inner_thread_body(std::forward<Func>(func));
			inner_thread_last();
		}

	public:
		std::thread &get_origin_thread() {
			return inner_thread_;
		}

		int get_cpu_id() const {
			return cpu_id_;
		}

		int get_numa_id() const {
			return numa_id_;
		}

	};

}

#endif//ALGORITHM_THREAD_NUMA_BIND_THREAD_H
