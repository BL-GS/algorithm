/*
 * @author: BL-GS 
 * @date:   2023/5/26
 */

/*!
 * @brief Get pmem performance by instruction 'impctl'
 * MediaReads: Number of 64 byte reads from media on the module since last AC cycle.
 * MediaWrites: Number of 64 byte writes to media on the module since last AC cycle.
 * ReadRequests: Number of DDRT read transactions the module has serviced since last AC cycle.
 * WriteRequests: Number of DDRT write transactions the module has serviced since last AC cycle.
 * TotalMediaReads: Number of 64 byte reads from media on the module over its lifetime.
 * TotalMediaWrites: Number of 64 byte writes to media on the module over its lifetime.
 * TotalReadRequests: Number of DDRT read transactions the module has serviced over its lifetime.
 * TotalWriteRequests: Number of DDRT write transactions the module has serviced over its lifetime.
 *
 * For example :
 * $ ipmctl show -performance
 *
	---DimmID=0x0011---
	   MediaReads=0x00000000000000000000001580458028
	   MediaWrites=0x0000000000000000000000115d003898
	   ReadRequests=0x00000000000000000000000005f3ce3c
	   WriteRequests=0x00000000000000000000000005e952d8
	   TotalMediaReads=0x00000000000000000000035eb4d474a8
	   TotalMediaWrites=0x0000000000000000000001810fb89564
	   TotalReadRequests=0x00000000000000000000019ff9ea6740
	   TotalWriteRequests=0x000000000000000000000125eaeef513
	---DimmID=0x0021---
	   MediaReads=0x0000000000000000000000157c998420
	   MediaWrites=0x0000000000000000000000115ba3d21c
	   ReadRequests=0x00000000000000000000000005f35b58
	   WriteRequests=0x00000000000000000000000005e94729
	   TotalMediaReads=0x00000000000000000000030cb06b4278
	   TotalMediaWrites=0x000000000000000000000152ef8c6250
	   TotalReadRequests=0x000000000000000000000167158c500a
	   TotalWriteRequests=0x0000000000000000000000e3c535bfe1
 */


#pragma once
#ifndef ALGORITHM_LISTENER_PMEM_LISTENER_H
#define ALGORITHM_LISTENER_PMEM_LISTENER_H

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <thread>
#include <chrono>
#include <regex>
#include <fstream>
#include <cassert>
#include <cstring>
#include <immintrin.h>

#include <logger/logger.h>
#include <listener/listener_interface.h>

namespace algorithm::listener {

	namespace detail {
		/*!
		 * @brief We just need the information about whole lifetime
		 */
		enum class PMMDataType: int {
			TotalMediaReads    = 0,
			TotalMediaWrites   = 1,
			TotalReadRequests  = 2,
			TotalWriteRequests = 3
		};

		/*!
	 * @brief Structure storing information about reading/writing
	 */
		struct DIMMData {
			uint64_t media_read;
			uint64_t media_write;
			uint64_t imc_read;
			uint64_t imc_write;

			DIMMData operator+ (const DIMMData &other) const {
				return {
				        .media_read  = media_read + other.media_read,
				        .media_write = media_write + other.media_write,
				        .imc_read    = imc_read + other.imc_read,
				        .imc_write   = imc_write + other.imc_write
				};
			}

			DIMMData &operator+= (const DIMMData &other) {
				media_read  += other.media_read;
				media_write += other.media_write;
				imc_read    += other.imc_read;
				imc_write   += other.imc_write;
				return *this;
			}

			DIMMData operator- (const DIMMData &other) const {
				return {
				        .media_read  = media_read - other.media_read,
				        .media_write = media_write - other.media_write,
				        .imc_read    = imc_read - other.imc_read,
				        .imc_write   = imc_write - other.imc_write
				};
			}

			DIMMData &operator-= (const DIMMData &other) {
				media_read  -= other.media_read;
				media_write -= other.media_write;
				imc_read    -= other.imc_read;
				imc_write   -= other.imc_write;
				return *this;
			}
		};

		/*!
	 * @brief Structure of information about DIMM
	 */
		struct DIMMObj {
		public:
			/// Name of DIMM
			std::string dimm_id_;
			/// Relative data of this DIMM
			DIMMData data_;
		};

		/*!
	 * @brief Structure expressing 128-bit number
	 */
		class DIMMAttribute128b {
		public:
			/// Higher 64 bits
			uint64_t h_u64b;
			/// Lower 64 bits
			uint64_t l_u64b;

			DIMMAttribute128b operator+ (const DIMMAttribute128b &other) const {
				return {
				        .h_u64b = h_u64b + other.h_u64b,
				        .l_u64b = l_u64b + other.l_u64b
				};
			}

			void operator+= (const DIMMAttribute128b &other) {
				h_u64b += other.h_u64b;
				l_u64b += other.l_u64b;
			}

			DIMMAttribute128b operator- (const DIMMAttribute128b &other) {
				return {
				        .h_u64b = h_u64b - other.h_u64b,
				        .l_u64b = l_u64b - other.l_u64b
				};
			}

			void operator-= (const DIMMAttribute128b &other) {
				h_u64b -= other.h_u64b;
				l_u64b -= other.l_u64b;
			}
		};

		/*!
	 * @brief Container for DIMM information
	 */
		class DIMMDataContainer {
		private:
			static constexpr uint32_t DATA_TYPE_NUM = 4;

		public:
			std::string dimm_id_;

			DIMMAttribute128b stat_list_[DATA_TYPE_NUM];

			DIMMDataContainer &operator= (const DIMMDataContainer &other) = default;

			DIMMDataContainer &operator+= (const DIMMDataContainer &other) {
				for (uint32_t i = 0; i < DATA_TYPE_NUM; ++i) {
					stat_list_[i] += other.stat_list_[i];
				}
				return *this;
			}

			DIMMDataContainer operator+ (const DIMMDataContainer &other) const {
				DIMMDataContainer new_container = *this;
				new_container += other;
				return new_container;
			}

			DIMMDataContainer &operator-= (const DIMMDataContainer &other) {
				for (uint32_t i = 0; i < DATA_TYPE_NUM; ++i) {
					stat_list_[i] -= other.stat_list_[i];
				}
				return *this;
			}

			DIMMDataContainer operator- (const DIMMDataContainer &other) const {
				DIMMDataContainer new_container = *this;
				new_container -= other;
				return new_container;
			}

			DIMMAttribute128b &get_stat(PMMDataType type) {
				return stat_list_[static_cast<int>(type)];
			}

			DIMMObj get_obj() {
				return {
				        .dimm_id_    = dimm_id_,
				        .data_ = {
				                .media_read  = get_stat(PMMDataType::TotalMediaReads).l_u64b,
				                .media_write = get_stat(PMMDataType::TotalMediaWrites).l_u64b,
				                .imc_read    = get_stat(PMMDataType::TotalReadRequests).l_u64b,
				                .imc_write   = get_stat(PMMDataType::TotalWriteRequests).l_u64b
				        }
				};
			}
		};


		/*!
		 * @brief Storage for all DIMM's data.
		 */
		class PMMData {
		public:
			static constexpr uint32_t DATA_TYPE_NUM = 4;

			static constexpr std::string_view TEMP_FILE_PATH = "/dev/shm/PMMData_temp";

			static constexpr std::string_view IPMCTL_INSTRUMENT = "ipmctl show -performance > /dev/shm/PMMData_temp";

			static constexpr std::string_view INFO_HEADER_PATTERN = R"(DimmID=0x([0-9a-f]*))";

			static constexpr std::array<std::string_view, DATA_TYPE_NUM> INFO_CONTENT_PATTERN = {
			        R"(^TotalMediaReads=0x([0-9a-f]*))",
			        R"(^TotalMediaWrites=0x([0-9a-f]*))",
			        R"(^TotalReadRequests=0x([0-9a-f]*))",
			        R"(^TotalWriteRequests=0x([0-9a-f]*))"};

			static constexpr std::string_view STAT_BIT_CONVERT_PATTERN = R"(^([0-9a-f]{16})([0-9a-f]{16}))";

		private:
			std::regex info_header_reg_;

			std::array<std::regex, DATA_TYPE_NUM> info_content_reg_;

			std::regex stat_bit_convert_reg_;

		public:
			std::vector<DIMMDataContainer> pmm_dimms_;

		public:
			PMMData():
			            info_header_reg_(INFO_HEADER_PATTERN.data()),
			            stat_bit_convert_reg_(STAT_BIT_CONVERT_PATTERN.data()) {
				for (uint32_t i = 0; i < DATA_TYPE_NUM; ++i) {
					info_content_reg_[i] = std::regex(INFO_CONTENT_PATTERN[i].data());
				}
			}

			PMMData &operator= (const PMMData &other) {
				pmm_dimms_.resize(other.pmm_dimms_.size());
				std::copy(other.pmm_dimms_.begin(), other.pmm_dimms_.end(), pmm_dimms_.begin());
				return *this;
			}

			PMMData &operator-= (const PMMData &other) {
				assert(pmm_dimms_.size() == other.pmm_dimms_.size());
				for (uint32_t i = 0; i < pmm_dimms_.size(); ++i) {
					pmm_dimms_[i] -= other.pmm_dimms_[i];
				}
				return *this;
			}

			PMMData operator- (const PMMData &other) const {
				auto new_container = *this;
				new_container -= other;
				return new_container;
			}

		public:

			void get_pmm_data() {
				pmm_dimms_.clear();

				// Get relative information
				std::system(IPMCTL_INSTRUMENT.data());
				std::ifstream ipmctl_stat(TEMP_FILE_PATH.data()); // open the input file

				std::string str_line;
				std::smatch matched_data;
				std::smatch matched_num;

				while (ipmctl_stat >> str_line) {
					// Match DIMM info block
					if (std::regex_search(str_line, matched_data, info_header_reg_)) {
						// Add new dimm object
						pmm_dimms_.emplace_back();
						pmm_dimms_.back().dimm_id_ = matched_data[1];
						// Scan and add all necessary data
						for (uint32_t i = 0; i < DATA_TYPE_NUM; ++i) {
							auto &data_reg = info_content_reg_[i];
							auto index_type = static_cast<PMMDataType>(i);

							while (ipmctl_stat >> str_line) {
								if (std::regex_search(str_line, matched_data, data_reg)) {
									std::string str128b = matched_data[1];
									if (std::regex_search(str128b, matched_num, stat_bit_convert_reg_)) {
										pmm_dimms_.back().get_stat(index_type).h_u64b = std::stoull(matched_num[1], nullptr, 16);
										pmm_dimms_.back().get_stat(index_type).l_u64b = std::stoull(matched_num[2], nullptr, 16);
									}
									else {
										perror("parse dimm stat");
										exit(EXIT_FAILURE);
									}
									break;
								}
							}
						}
					}
				}
			}
		};


		/*!
	 * @brief Collector of DIMM data.
	 */
		class PMMDataCollector {
		private:
			PMMData start_data_;

			PMMData end_data_;

			std::chrono::steady_clock::time_point start_timer_;

			std::chrono::steady_clock::time_point end_timer_;

			std::vector<DIMMObj> dimm_info_list_;

		public:
			explicit PMMDataCollector(bool start_flag = false) {
				if (start_flag) { start_record(); }
			}

		public:
			void start_record() {
				start_data_.get_pmm_data();
				end_data_ = start_data_;
				start_timer_ = std::chrono::steady_clock::now();
			}

			void end_record() {
				end_data_.get_pmm_data();
				end_timer_ = std::chrono::steady_clock::now();
			}

			void install_record() {
				start_data_  = end_data_;
				start_timer_ = end_timer_;
				end_timer_   = std::chrono::steady_clock::now();
				end_data_.get_pmm_data();
			}

			void calculate_record() {
				auto res_data = end_data_ - start_data_;
				dimm_info_list_.clear();
				for (auto &dimm_info: res_data.pmm_dimms_) {
					dimm_info_list_.emplace_back(dimm_info.get_obj());
				}
			}

			uint64_t get_listen_time() {
				return std::chrono::duration_cast<std::chrono::milliseconds>(end_timer_ - start_timer_).count();
			}

			DIMMData get_data() {
				DIMMData res_data = {0, 0, 0, 0};
				for (auto &dimm_info: dimm_info_list_) {
					res_data += dimm_info.data_;
				}
				res_data.imc_read -= res_data.imc_write;
				return res_data;
			}

		};
	}


	/*!
	 * @brief Listener of DIMM data.
	 * Using a separate thread recording data.
	 */
	class PMMListener: AbstractListener {
	private:
		static constexpr std::string_view RECORD_FILE_PATH = "PMMListener.csv";

		static constexpr std::chrono::milliseconds interval{500};

	private:
		detail::PMMDataCollector data_collector_;

		std::atomic_flag stop_flag_;

		std::ofstream record_file_;

	public:
		PMMListener():
              data_collector_(true),
              stop_flag_(false),
              record_file_(RECORD_FILE_PATH.data(), std::ios::app) {
			record_file_ << "Index, IMC read, IMC write, Media read, Media write\n";

			util::logger::logger_info("PMMListener: record file path = ", RECORD_FILE_PATH);
		}

		~PMMListener() = default;

	public:
		void start_record() {
			std::thread listener_thread_(
		        [&](){
			        uint32_t index = 0;

			        auto start = std::chrono::steady_clock::now();
			        auto end = start;
			        while (!stop_flag_.test()) {
				        if (end - start >= interval) {
					        data_collector_.install_record();
					        data_collector_.calculate_record();
					        auto data = data_collector_.get_data();
					        record_file_    << index++          << ','
					                     << data.imc_read    << ','
					                     << data.imc_write   << ','
					                     << data.media_read  << ','
					                     << data.media_write << '\n';
					        end = start;
				        }
				        std::this_thread::yield();
			        }
		        }
	        );
		}

		void end_record() {
			stop_flag_.test_and_set();
			record_file_.flush();
			record_file_.close();
		}
	};
}

#endif//ALGORITHM_LISTENER_PMEM_LISTENER_H
