/*
 * @author: BL-GS 
 * @date:   2023/5/23
 */

#pragma once
#ifndef UTIL_ALGORITHM_THREAD_H
#define UTIL_ALGORITHM_THREAD_H

#include <vector>
#include <atomic>
#include <mutex>

#include <sys/sysinfo.h>

#include <memory/numa.h>
#include <memory/thread_config.h>
#include <logger/logger.h>

namespace algorithm::memory {

	class ThreadConfig {
	private:
		int num_cpu_;

		std::atomic<bool> logic_tid_storage_[MAX_TID];

		int tid_to_cpu_id_[MAX_TID];

		std::atomic<bool> *cpu_id_storage_;

		std::vector<int> cpu_id_to_numa_;

		std::vector<int> num_cpu_on_node_;

	public:
		ThreadConfig():  num_cpu_(max_cpu_num()),
		                 cpu_id_storage_(new std::atomic<bool>[num_cpu_]),
		                 cpu_id_to_numa_(num_cpu_, -1) {
			if (num_cpu_ < MAX_TID) {
				util::logger::logger_warn("Macro MAX_TID is larger than the number of available cpus.");
			}

			int num_numa_node = NUMA_CONFIG.get_max_numa_node();

			std::fill(logic_tid_storage_, logic_tid_storage_ + MAX_TID, false);
			std::fill(tid_to_cpu_id_, tid_to_cpu_id_ + MAX_TID, -1);

			for (int numa_id = 0; numa_id < num_numa_node; ++numa_id) {
				auto cpu_mask = NUMAConfig::get_cpu_per_node(numa_id);
				num_cpu_on_node_.emplace_back(0);

				for (int cpu_id = 0; cpu_id < num_cpu_; ++cpu_id) {
					if (cpu_mask.is_set(cpu_id)) {
						cpu_id_to_numa_[cpu_id] = numa_id;
						num_cpu_on_node_[numa_id]++;
					}
				}
			}
		}

		~ThreadConfig() {
			delete[] cpu_id_storage_;
		}

	public:
		[[nodiscard]] bool is_tid_available(int tid) const {
			return logic_tid_storage_[tid].load(std::memory_order::acquire);
		}

		bool bind_tid(int tid) {
			bool origin = false;
			return logic_tid_storage_[tid].compare_exchange_strong(origin, true);
		}

		int allocate_tid() {
			bool origin = false;
			for (int i = 0; i < MAX_TID; ++i) {
				if (!logic_tid_storage_[i].load(std::memory_order::relaxed)) {
					if (logic_tid_storage_[i].compare_exchange_strong(origin, true)) {
						return i;
					}
				}
			}
			util::logger::logger_error("No left TID.");
			return -1;
		}

		void deallocate_tid(int tid) {
			bool origin = true;
			if (!logic_tid_storage_[tid].compare_exchange_strong(origin, false)) {
				util::logger::logger_error("Double deallocate tid ", tid);
			}
			if (tid_to_cpu_id_[tid] != -1) {
				deallocate_cpu(tid);
			}
		}

		static constexpr int get_max_tid() {
			return MAX_TID;
		}

	public:
		int get_num_cpu_on_node(int numa_id) {
			return num_cpu_on_node_[numa_id];
		}

		int get_cpu_id_by_tid(int tid) {
			return tid_to_cpu_id_[tid];
		}

		int allocate_cpu_on_node(int tid, int numa_id) {

			for (int cpu_id = 0; cpu_id < num_cpu_; ++cpu_id) {
				if (cpu_id_to_numa_[cpu_id] == numa_id) {
					bool origin = false;
					if (cpu_id_storage_[cpu_id].compare_exchange_strong(origin, true)) {
						tid_to_cpu_id_[tid] = cpu_id;
						return cpu_id;
					}
				}
			}
			return -1;
		}

		std::pair<int, int> allocate_cpu(int tid) {
			for (int cpu_id = 0; cpu_id < num_cpu_; ++cpu_id) {
				if (!cpu_id_storage_[cpu_id].load()) {
					bool origin = false;
					if (cpu_id_storage_[cpu_id].compare_exchange_strong(origin, true)) {
						tid_to_cpu_id_[tid] = cpu_id;
						return { cpu_id_to_numa_[cpu_id], cpu_id };
					}
				}
			}
			return { -1, -1 };
		}

		void deallocate_cpu(int tid) {
			int cpu_id = tid_to_cpu_id_[tid];
			tid_to_cpu_id_[tid] = -1;

			bool origin = true;
			if (!cpu_id_storage_[cpu_id].compare_exchange_strong(origin, false)) {
				util::logger::logger_error("Deallocate cpu id before allocating.");
			}
		}

		[[nodiscard]] int get_cpu_num() const {
			return num_cpu_;
		}

		[[nodiscard]] int get_cpu_numa_id(int tid) const {
			return cpu_id_to_numa_[tid_to_cpu_id_[tid]];
		}

	public:
		static void bind_cpu(int cpu_id) {
			cpu_set_t cpu_set;
			CPU_ZERO(&cpu_set);
			CPU_SET(cpu_id, &cpu_set);
			sched_setaffinity(0, sizeof(cpu_set_t), &cpu_set);
		}

	private:
		static int max_cpu_num() {
			return get_nprocs();
		}
	};

	inline static ThreadConfig THREAD_CONFIG;

	class ThreadInfo {
	private:
		int tid_;

	public:
		ThreadInfo(): tid_(-1) {}

		~ThreadInfo() {
			if (tid_ != -1) {
				THREAD_CONFIG.deallocate_tid(tid_);
			}
		}

	public:
		[[nodiscard]] static bool is_tid_available(int tid) {
			return THREAD_CONFIG.is_tid_available(tid);
		}

		bool bind_tid(int tid) {
			if (THREAD_CONFIG.bind_tid(tid)) {
				tid_ = tid;
				return true;
			}
			return false;
		}

		int allocate_tid() {
			tid_ = THREAD_CONFIG.allocate_tid();
			return tid_;
		}

		void deallocate_tid() {
			THREAD_CONFIG.deallocate_tid(tid_);
			tid_ = -1;
		}

		[[nodiscard]] int get_tid() const {
			return tid_;
		}

		static int get_max_tid() {
			return THREAD_CONFIG.get_max_tid();
		}

	public:
		static int get_num_cpu_on_node(int numa_id) {
			return THREAD_CONFIG.get_num_cpu_on_node(numa_id);
		}

		[[nodiscard]] static int get_cpu_num() {
			return THREAD_CONFIG.get_cpu_num();
		}

		[[nodiscard]] int get_cpu_id_by_tid() const {
			return THREAD_CONFIG.get_cpu_id_by_tid(tid_);
		}

		[[nodiscard]] int bind_cpu_on_node(int numa_id) const {
			int cpu_id = THREAD_CONFIG.allocate_cpu_on_node(tid_, numa_id);
			ThreadConfig::bind_cpu(cpu_id);
			return cpu_id;
		}

		[[nodiscard]] std::pair<int, int> bind_cpu() const {
			auto [numa_id, cpu_id] = THREAD_CONFIG.allocate_cpu(tid_);
			ThreadConfig::bind_cpu(cpu_id);
			return { numa_id, cpu_id };
		}

	};

	inline thread_local ThreadInfo THREAD_CONTEXT;

	/*!
	 * @brief Get the upperbound of tid
	 */
	inline constexpr uint32_t get_max_tid() {
		return ThreadConfig::get_max_tid();
	}

	/*!
	 * @brief Get the tid of the current thread
	 */
	inline uint32_t get_tid() {
		return THREAD_CONTEXT.get_tid();
	}

	/*!
	 * @brief Query whether the current thread has register a tid.
	 */
	inline bool is_registered() {
		return THREAD_CONTEXT.get_tid() != -1;
	}

	/*!
	 * @brief Get the numa id of the current cpu
	 */
	inline uint32_t get_cpu_numa_id() {
		return THREAD_CONFIG.get_cpu_numa_id(THREAD_CONTEXT.get_tid());
	}

}


#endif//UTIL_ALGORITHM_THREAD_H
