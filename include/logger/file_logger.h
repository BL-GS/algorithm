/*
 * @author: BL-GS 
 * @date:   2023/4/16
 */

#pragma once
#ifndef ALGORITHM_UTIL_LOGGER_FILE_LOGGER_H
#define ALGORITHM_UTIL_LOGGER_FILE_LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include <logger/abstract_logger.h>

namespace algorithm::util::logger {

	/// @brief File name for logger output
	#ifndef LOGGER_OUTPUT_FILE_NAME
		#define LOGGER_OUTPUT_FILE_NAME "Logger.csv"
	#endif

	template<bool coloring>
	class Logger<Output_Type::FILE, coloring> : public LoggerBase {
	private:
		using Self = Logger<Output_Type::FILE, coloring>;

	private:
		std::string name_;

		std::ofstream output_stream_;

		std::string property_prefix_;

		std::map<std::string, std::vector<std::string>> table_;

	private:
		explicit Logger(std::string_view logger_name = LOGGER_OUTPUT_FILE_NAME) : name_(logger_name),
		                                                                          output_stream_(name_, std::ios::app) {
			info("Logger output file: " + name_);
		}

	public:
		Logger(const Logger &other) = delete;
		Logger(Logger &&other) = delete;

		~Logger() {
			if (output_stream_.is_open()) {
				print_table();
				output_stream_.flush();
				output_stream_.close();
			}
		}

		//! Singleton: Get the only instance
		//! \return
		static inline Self &get_instance() {
			static Self instance_;
			return instance_;
		}

	public:// ---------------- High-Level Function
		template<class... Args>
		void print_property(std::string_view header_name, Args &&...left_property) {
			print_property_header(header_name);
			_print_property(std::forward<Args>(left_property)...);
			print_delimiter_line();
		}

		template<class... Arg>
		void error(Arg &&...args) {
			error_log();
			print_lot(std::forward<Arg>(args)...);
			error_end();
		}

		template<class... Arg>
		void warn(Arg &&...args) {
			warn_log();
			print_lot(std::forward<Arg>(args)...);
			warn_end();
		}

		template<class... Arg>
		void info(Arg &&...args) {
			info_log();
			print_lot(std::forward<Arg>(args)...);
			info_end();
		}

		template<class... Arg>
		void error_format(const char *format, Arg &&...args) {
			error_log();
			print_lot_format(format, std::forward<Arg>(args)...);
			error_end();
		}

		template<class... Arg>
		void warn_format(const char *format, Arg &&...args) {
			warn_log();
			print_lot_format(format, std::forward<Arg>(args)...);
			warn_end();
		}

		template<class... Arg>
		void info_format(const char *format, Arg &&...args) {
			info_log();
			print_lot_format(format, std::forward<Arg>(args)...);
			info_end();
		}

	public:// ---------------- Basic Function
		void print_property_header(std::string_view header_name) {
			property_prefix_ = '[' + std::string(header_name) + ']';
		}

		template<class V>
		void print_kv_pair(std::string_view key, const V &value, std::string_view unit = "") {
			std::string key_obj = property_prefix_ + key.data() + '(' + unit.data() + ')';
			std::string value_str = std::to_string(value);
			std::string unit_str = std::string(unit);

			if (!table_.contains(key_obj)) {
				table_.emplace(key_obj, std::vector<std::string>());
			}
			table_[key_obj].emplace_back(value_str);
		}

		void print_kv_pair(std::string_view key, const std::string &value_str, std::string_view unit = "") {
			std::string key_obj = property_prefix_ + key.data() + '(' + unit.data() + ')';
			std::string unit_str = std::string(unit);

			if (!table_.contains(key_obj)) {
				table_.emplace(key_obj, std::vector<std::string>());
			}
			table_[key_obj].emplace_back(value_str);
		}

		void print_delimiter_line() {}

		template<class V>
		void print(const V &obj, Background_Color bc = Background_Color::NONE,
		           Font_Color fc = Font_Color::NONE,
		           Effect e = Effect::NONE) {
			std::cout << prefix(bc, fc, e)
			          << obj
			          << suffix();
		}

		void print_lot() {
			std::cout << '\n';
		}

		template<class T, class... Arg>
		void print_lot(T &&content, Arg &&...args) {
			std::cout << content;
			print_lot(std::forward<Arg>(args)...);
		}

		template<class... Arg>
		void print_lot_format(const char *format, Arg &&...args) {
			std::printf(format, std::forward<Arg>(args)...);
		}

		Logger &output_log() {
			if constexpr (coloring) {
				std::cout << prefix_static<Background_Color::NONE, Font_Color::GREEN, Effect::NONE>();
			}
			return *this;
		}

		Logger &output_end() {
			if constexpr (coloring) {
				std::cout << suffix();
			}
			return *this;
		}

		Logger &info_log() {
			if constexpr (coloring) {
				std::cout << prefix_static<Background_Color::NONE, Font_Color::BLUE, Effect::NONE>() << "[Info] ";
			}
			return *this;
		}

		Logger &info_end() {
			if constexpr (coloring) {
				std::cout << suffix();
			}
			return *this;
		}

		Logger &warn_log() {
			if constexpr (coloring) {
				std::cout << prefix_static<Background_Color::NONE, Font_Color::YELLOW, Effect::HIGHLIGHT>() << "[Warning] ";
			}
			return *this;
		}

		Logger &warn_end() {
			if constexpr (coloring) {
				std::cout << suffix();
			}
			return *this;
		}

		Logger &error_log() {
			if constexpr (coloring) {
				std::cout << prefix_static<Background_Color::NONE, Font_Color::RED, Effect::HIGHLIGHT>() << "[Error] ";
			}
			return *this;
		}

		Logger &error_end() {
			if constexpr (coloring) {
				std::cout << suffix();
			}
			return *this;
		}

	private:
		inline void _print_property() {}

		template<class T1, class T2, class T3, class... Args>
		void _print_property(std::tuple<T1, T2, T3> &&cur_property, Args &&...left_property) {
			auto [first, second, third] = cur_property;
			print_kv_pair(first, second, third);
			_print_property(std::forward<Args>(left_property)...);
		}

		void print_table() {
			bool first_property = true;

			// print properties
			for (auto &pair: table_) {
				if (!first_property) { output_stream_ << ','; }
				else { first_property = false; }
				output_stream_ << pair.first;
			}
			output_stream_ << '\n';

			// print value

			for (uint32_t iter = 0; iter < table_.begin()->second.size(); ++iter) {
				first_property = true;
				for (auto &pair: table_) {
					if (!first_property) { output_stream_ << ','; }
					else { first_property = false; }

					output_stream_ << pair.second[iter];
				}
				output_stream_ << '\n';
			}
		}
	};

}

#endif//ALGORITHM_UTIL_LOGGER_FILE_LOGGER_H
