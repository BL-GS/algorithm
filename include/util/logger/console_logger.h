/*
 * @author: BL-GS 
 * @date:   2023/4/16
 */

#pragma once
#ifndef ALGORITHM_UTIL_LOGGER_CONSOLE_LOGGER_H
#define ALGORITHM_UTIL_LOGGER_CONSOLE_LOGGER_H

	#include <iostream>
	#include <string>
	#include <iomanip>

	#include <util/logger/abstract_logger.h>

namespace algorithm::util::logger {

	template<bool coloring>
	class Logger<Output_Type::CONSOLE, coloring> : public LoggerBase {
	private:
		using Self = Logger<Output_Type::CONSOLE, coloring>;

		static constexpr std::string_view delimiter_line =
		        "--------------------------------------------------------------------";

	private:
		std::string name_;

	private:
		explicit Logger(std::string_view logger_name = "Logger") : name_(logger_name) {}

	public:
		~Logger() = default;

		//! Singleton: Get the only instance
		//! \return
		inline static Self &get_instance() {
			static Self instance_;
			return instance_;
		}

		/*
			 * ---------------- High-Level Function
			 */

	public:
		template<class... Args>
		void print_property(std::string_view header_name, Args &&...left_property) {
			print_property_header(header_name);
			output_log();
			_print_property(std::forward<Args>(left_property)...);
			output_end();
			print_delimiter_line();
		}

		template<class... Arg>
		void error(Arg &&...args) {
			error_log();
			print_lot(std::forward<Arg>(args)...);
			error_end();
			std::flush(std::cout);
		}

		template<class... Arg>
		void warn(Arg &&...args) {
			warn_log();
			print_lot(std::forward<Arg>(args)...);
			warn_end();
			std::flush(std::cout);
		}

		template<class... Arg>
		void info(Arg &&...args) {
			info_log();
			print_lot(std::forward<Arg>(args)...);
			info_end();
			std::flush(std::cout);
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
			if constexpr (coloring) {
				std::cout << prefix_static<Background_Color::NONE, Font_Color::YELLOW, Effect::HIGHLIGHT>()
				          << "[ " << name_ << ": " << header_name << " ]"
				          << delimiter_line
				          << suffix()
				          << '\n';
			}
			else {
				std::cout << "[ " << name_ << ": " << header_name << " ]"
				          << delimiter_line
				          << '\n';
			}
		}

		void print_delimiter_line() {
			std::cout << '\n'
			          << delimiter_line << '\n';
			std::flush(std::cout);
		}

		template<class V>
		void print_kv_pair(std::string_view key, const V &value, std::string_view unit = "") {
			std::cout << std::left << std::setw(36) << key << '\t'
			          << std::setw(24) << value << '\t'
			          << unit << '\n';
		}

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
		void print_lot(T &content, Arg &&...args) {
			std::cout << content;
			print_lot(args...);
		}

		template<class... Arg>
		void print_lot_format(const char *format, Arg &&...args) {
			std::printf(format, std::forward<Arg>(args)...);
		}

	private:
		void _print_property() {}

		template<class T1, class T2, class T3, class... Args>
		void _print_property(std::tuple<T1, T2, T3> &&cur_property, Args &&...left_property) {
			auto [first, second, third] = cur_property;
			print_kv_pair(first, second, third);
			_print_property(std::forward<Args>(left_property)...);
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
	};
}

#endif//ALGORITHM_UTIL_LOGGER_CONSOLE_LOGGER_H
