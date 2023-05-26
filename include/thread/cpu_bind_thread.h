/*
 * @author: BL-GS 
 * @date:   2023/5/26
 */

#pragma once
#ifndef ALGORITHM_THREAD_CPU_BIND_THREAD_H
#define ALGORITHM_THREAD_CPU_BIND_THREAD_H

#include <thread>

#include <memory/memory.h>
#include <logger/logger.h>

namespace algorithm::thread {

	class CPUBindThread {
	public:
		static constexpr int INVALID_ID = -1;

	private:
		int cpu_id_  = INVALID_ID;

		int numa_id_ = INVALID_ID;

		std::thread inner_thread_;

	public:
		template<class Func>
		CPUBindThread(Func &&func): inner_thread_(
           [inner_func = std::forward<Func>(func), this] {
               inner_thread_func(inner_func);
           }) {}

		template<class Func>
		CPUBindThread(int tid, Func &&func): inner_thread_(
          [tid, inner_func = std::forward<Func>(func), this] {
              inner_thread_func(tid, inner_func);
          }) {}


	private:
		void inner_thread_preload() {
			memory::THREAD_CONTEXT.allocate_tid();
			std::tie(numa_id_, cpu_id_) = memory::THREAD_CONTEXT.bind_cpu();
		}

		void inner_thread_preload(int tid) {
			if (!memory::THREAD_CONTEXT.bind_tid(tid)) {
				util::logger::logger_error("Unable to bind tid ", tid);
			}
			std::tie(numa_id_, cpu_id_) = memory::THREAD_CONTEXT.bind_cpu();
		}

		template<class Func>
		static void inner_thread_body(Func &func) {
			func();
		}

		static void inner_thread_last() {}

		template<class Func>
		void inner_thread_func(Func &func) {
			inner_thread_preload();
			inner_thread_body(std::forward<Func>(func));
			inner_thread_last();
		}

		template<class Func>
		void inner_thread_func(int tid, Func &func) {
			inner_thread_preload(tid);
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

#endif//ALGORITHM_THREAD_CPU_BIND_THREAD_H
