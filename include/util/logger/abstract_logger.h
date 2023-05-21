/*
 * @author: BL-GS
 * @date: 2023.05.03
 */
#pragma once
#ifndef ALGORITHM_UTIL_LOGGER_ABSTRACT_LOGGER_H
#define ALGORITHM_UTIL_LOGGER_ABSTRACT_LOGGER_H

#include <string>
#include <string_view>

namespace algorithm::util::logger {

	enum class Output_Type {
		CONSOLE,
		FILE
	};

	enum class Background_Color {
		BLACK     = 40,
		RED       = 41,
		GREEN     = 42,
		YELLOW    = 43,
		BLUE      = 44,
		PURPLE    = 45,
		DARKGREEN = 46,
		GRAY      = 47,
		NONE      = 48
	};

	enum class Font_Color {
		BLACK     = 30,
		RED       = 31,
		GREEN     = 32,
		YELLOW    = 33,
		BLUE      = 34,
		PURPLE    = 35,
		DARKGREEN = 36,
		GRAY      = 37,
		NONE      = 38
	};

	enum class Effect {
		OFF       = 0,
		HIGHLIGHT = 1,
		LOWLIGHT  = 2,
		UNDERLINE = 4,
		BRINK     = 5,
		INVERSE   = 7,
		FADE      = 8,
		NONE      = 9
	};


	class LoggerBase {
	public:
		static constexpr std::string_view suffix() {
			return {"\033[0m"};
		}

		template<Background_Color bc, Font_Color fc, Effect e>
		static std::string prefix_static() {
			std::string s("\033[");
			if constexpr (bc != Background_Color::NONE) {
				s += std::to_string((int) bc);
				s += ';';
			}
			if constexpr (fc != Font_Color::NONE) {
				s += std::to_string((int) fc);
				s += ';';
			}
			if constexpr (e != Effect::NONE) {
				s += std::to_string((int) e);
				s += ';';
			}
			*(s.end() - 1) = 'm';
			return s;
		}

		static std::string prefix(Background_Color bc, Font_Color fc, Effect e) {
			std::string s("\033[");
			if (bc != Background_Color::NONE) {
				s += std::to_string((int) bc);
				s += ';';
			}
			if (fc != Font_Color::NONE) {
				s += std::to_string((int) fc);
				s += ';';
			}
			if (e != Effect::NONE) {
				s += std::to_string((int) e);
				s += ';';
			}
			*(s.end() - 1) = 'm';
			return s;
		}
	};

	template<Output_Type output_type, bool coloring>
	class Logger {};
}// namespace algorithm::util::logger

#endif