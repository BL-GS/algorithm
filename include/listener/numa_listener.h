/*
 * @author: BL-GS 
 * @date:   2023/5/24
 */

#pragma once
#ifndef ALGORITHM_LISTENER_NUMA_LISTENER_H
#define ALGORITHM_LISTENER_NUMA_LISTENER_H

#include <array>
#include <string>
#include <vector>
#include <utility>

#include <sys/unistd.h>
#include <numa.h>

#include <logger/logger.h>
#include <listener/listener_interface.h>

namespace algorithm::listener {

	struct NUMAWatcher: AbstractListener {
	public:
		const char *TEMP_FILE_NAME = "/dev/shm/NUMAWatcher";

		const char *NUMA_STAT_CMD = "numastat -n -p ";

		static constexpr std::array<std::string_view, 6> INFO_CONTENT_PATTERN = {
		        "numa_hit",
		        "numa_miss",
		        "numa_foreign",
		        "interleave_hit",
		        "local_node",
		        "other_node",
		};

	private:
		int numa_node_num_;

		std::string stat_cmd_;

		std::vector<float> numa_info_vec[INFO_CONTENT_PATTERN.size()];

	public:
		NUMAWatcher(int numa_node_num): numa_node_num_(numa_node_num) {
			if (!numa_available()) {
				util::logger::logger_warn(
				        "NUMA is not available in this system. Binding node may incur undefined results."
				);
			}

			for (auto &info_vec: numa_info_vec) {
				info_vec.resize(numa_node_num, 0);
			}

			stat_cmd_ += NUMA_STAT_CMD;
			stat_cmd_ += getpid();
			stat_cmd_ += " > ";
			stat_cmd_ += TEMP_FILE_NAME;
		}

		~NUMAWatcher() = default;

		void start_record() override {
			std::system(stat_cmd_.c_str());
			std::ifstream numa_stat_res(TEMP_FILE_NAME); // open the input file

			std::string temp_buffer;
			for (int i = 0; i < 14; ++i) {
				std::getline(numa_stat_res, temp_buffer);
			}

			for (auto &numa_info: numa_info_vec) {
				// Discard the information of index name
				numa_stat_res >> temp_buffer;
				// Get information of each node
				for (int node_id = 0; node_id < numa_node_num_; ++node_id) {
					numa_stat_res >> numa_info[node_id];
				}
				// Discard the information of 'total'
				numa_stat_res >> temp_buffer;
			}
		}

		void end_record() override {
			std::system(stat_cmd_.c_str());
			std::ifstream numa_stat_res(TEMP_FILE_NAME); // open the input file

			std::string temp_buffer;
			for (int i = 0; i < 14; ++i) {
				std::getline(numa_stat_res, temp_buffer);
			}

			for (auto &numa_info: numa_info_vec) {
				// Discard the information of index name
				numa_stat_res >> temp_buffer;
				// Get information of each node and differ with start status
				for (int node_id = 0; node_id < numa_node_num_; ++node_id) {
					float matched_res;
					numa_stat_res >> matched_res;
					numa_info[node_id] = matched_res - numa_info[node_id];
				}
				// Discard the information of 'total'
				numa_stat_res >> temp_buffer;
			}

			for (size_t node_id = 0; node_id < numa_node_num_; ++node_id) {
				util::logger::logger_print_property("NUMA Watcher",
				                                    std::make_tuple(INFO_CONTENT_PATTERN[0], numa_info_vec[0][node_id], "MB"),
				                                    std::make_tuple(INFO_CONTENT_PATTERN[1], numa_info_vec[1][node_id], "MB"),
				                                    std::make_tuple(INFO_CONTENT_PATTERN[2], numa_info_vec[2][node_id], "MB"),
				                                    std::make_tuple(INFO_CONTENT_PATTERN[3], numa_info_vec[3][node_id], "MB"),
				                                    std::make_tuple(INFO_CONTENT_PATTERN[4], numa_info_vec[4][node_id], "MB"),
				                                    std::make_tuple(INFO_CONTENT_PATTERN[5], numa_info_vec[5][node_id], "MB")
				);
			}
		}
	};

}

#endif//ALGORITHM_LISTENER_NUMA_LISTENER_H
