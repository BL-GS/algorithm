/*
 * @author: BL-GS 
 * @date:   2023/5/22
 */

#pragma once
#ifndef ALGORITHM_MEMORY_NUMA_H
#define ALGORITHM_MEMORY_NUMA_H

#include <fstream>
#include <string>
#include <charconv>
#include <array>

#include <sys/types.h>
#include <unistd.h>
#include <numa.h>
#include <sched.h>

#include <logger/logger.h>

namespace algorithm::memory {

	struct NUMAConfig {
	public:
		struct CPUBitmaskWrapper {
		private:
			/// Pointer to mask of cpus
			bitmask *cpu_mask_ptr_;

		public:
			CPUBitmaskWrapper(int node_id): cpu_mask_ptr_(numa_allocate_cpumask()) {
				// Allocate mask enough for this system
				numa_node_to_cpus(node_id, cpu_mask_ptr_);
			}

			CPUBitmaskWrapper(CPUBitmaskWrapper &&other) noexcept {
				cpu_mask_ptr_ = other.cpu_mask_ptr_;
				other.cpu_mask_ptr_ = nullptr;
			}

			~CPUBitmaskWrapper() {
				// Free mask if allocated.
				if (cpu_mask_ptr_ != nullptr) {
					numa_free_cpumask(cpu_mask_ptr_);
				}
			}

		public:
			/*!
			 * @brief Return whether the cpu with specific id is contained in this mask
			 * @param cpu_id The logic id of cpu
			 */
			[[nodiscard]] bool is_set(int cpu_id) const {
				return numa_bitmask_isbitset(cpu_mask_ptr_, cpu_id) != 0;
			}
		};

	private:
		/// Whether NUMA is available in this computer
		bool is_numa_available_;
		/// The number of nodes
		int numa_node_num_;

	public:
		NUMAConfig() {
			is_numa_available_ = (numa_available() != -1);
			numa_node_num_     = numa_num_configured_nodes();
		}

		~NUMAConfig() =default;

	public:
		/*!
		 * @brief Bind the current thread to specific numa node.
		 * @param node_id The id of numa node
		 */
		void bind_node(int node_id) {
			// Check numa availability
			numa_available_warn();
			// Bind numa to specific node
			numa_run_on_node(node_id);
			numa_set_preferred(node_id);
		}

		/*!
		 * @brief Unbind the current thread
		 */
		void unbind_node() {
			// Check numa availability
			numa_available_warn();
			// Unbind from numa node
			bitmask mask;
			numa_run_on_node_mask_all(&mask);
		}

	public:
		/*!
		 * @brief Acquire whether numa is available in the current system
		 */
		[[nodiscard]] bool is_numa_available() const {
			return is_numa_available_;
		}

		/*!
		 * @brief Acquire the max number of available nodes in the current system
		 * @return The max number of available nodes.
		 */
		[[nodiscard]] int get_max_numa_node() const {
			return numa_node_num_;
		}

		/*!
		 * @brief Acquire bitmask pointed cpus on specific numa node.
		 */
		static CPUBitmaskWrapper get_cpu_per_node(int node_id) {
			return {node_id};
		}

	private:
		/*!
		 * @brief Check whether numa is available in this system.
		 */
		bool numa_available_warn() const {
			if (!is_numa_available()) {
				util::logger::logger_warn(
				        "NUMA is not available in this system. Binding node may incur undefined results."
				);
				return false;
			}
			return true;
		}

	};

	/// Global configer about NUMA
	inline static NUMAConfig NUMA_CONFIG;

	/*!
	 * @brief Acquire whether numa is available in the current system
	 */
	inline bool is_numa_available() {
		return NUMA_CONFIG.is_numa_available();
	}

	/*!
	 * @brief Acquire the max number of available nodes in the current system
	 * @return The max number of available nodes.
	 */
	inline int get_max_numa_node() {
		return NUMA_CONFIG.get_max_numa_node();
	}

	/*!
	 * @brief Bind the current thread to specific numa node.
	 * @param node_id The id of numa node
	 */
	inline void bind_node(int numa_id) {
		NUMA_CONFIG.bind_node(numa_id);
	}

	/*!
	 * @brief Unbind the current thread
	 */
	inline void unbind_node() {
		NUMA_CONFIG.unbind_node();
	}

}

#endif//ALGORITHM_MEMORY_NUMA_H
