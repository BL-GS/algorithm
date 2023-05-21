/*
 * @author: BL-GS 
 * @date:   2022/12/1
 */

#pragma once
#ifndef UTIL_CLASS_H
#define UTIL_CLASS_H

#include <string>

namespace util {

	//! Get the total name at compilation time.
	//! \tparam T The type to be print
	//! \tparam N The instance of type T
	//! \return The C-type string of type's name.
	template <class T>
	inline constexpr const char *get_class_name_static([[maybe_unused]] const T &obj) {
#if defined(_MSC_VER)
		return __FUNCSIG__;
#else
		return __PRETTY_FUNCTION__;
#endif
	}

	//! Get the name of class about concrete object
	//! \tparam T The type of the object.
	//! \tparam Beg The begin number to begin searching.
	//! \tparam End The end number to end searching.
	//! \param target The object to be detected.
	//! \return The name of class.
	template <class T, int Beg, int End>
	inline constexpr std::string_view get_class_name(const T &target) {
		std::string_view s = get_class_name_static(target);
		if (s.empty()) { return ""; }
#if defined(_MSC_VER)
		size_t pos = s.find(',');
		pos += 1;
		size_t pos2 = s.find('>', pos);
#else
		size_t pos = s.find("T = ");
		pos += 4;
		size_t pos2 = s.find_first_of(";]", pos);
#endif
		s			= s.substr(pos, pos2 - pos);
		size_t pos3 = s.find("::");
		if (pos3 != std::string_view::npos) { s = s.substr(pos3 + 2); }
		return s;
	}

	//! Get the name of class about concrete object
	//! \tparam T The type of the object.
	//! \param target The object to be detected.
	//! \return The name of class.
	template <class T>
	inline std::string get_class_name(const T &target) {
		return std::string(get_class_name<T, 0, 256>(target));
	}
}

#endif //PTM_CLASS_H
