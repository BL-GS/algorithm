/*
 * @author: BL-GS 
 * @date:   2022/12/1
 * @Ref: https://github.com/parallel101/course/blob/master/specmacro/scienum.h
 */

#pragma once
#ifndef REFLECTION_ENUM_H
#define REFLECTION_ENUM_H

#include <string>

#include <util/static_for.h>

namespace util {

	namespace algorithm::reflect {

		//! Get the total name at compilation time.
		//! \tparam T The type to be print
		//! \tparam N The instance of type T
		//! \return The C-type string of type's name.
		template <class T, T N>
		inline constexpr const char *get_enum_name_static() {
	#if defined(_MSC_VER)
			return __FUNCSIG__;
	#else
			return __PRETTY_FUNCTION__;
	#endif
		}

		//! The reactor for enum name.
		//! \tparam T The type to be reacted for.
		template <class T>
		struct get_enum_name_functor {
			int			 n;
			std::string_view &s;

			get_enum_name_functor(int n, std::string_view &s) : n(n), s(s) {}

			template <int I>
			void call() const {
				if (n == I) { s = get_enum_name_static<T, (T)I>(); }
			}
		};

		//! Get the name of enum about concrete object
		//! \tparam T The type of the object.
		//! \tparam Beg The begin number to begin searching.
		//! \tparam End The end number to end searching.
		//! \param n The object to be detected.
		//! \return The name of object.
		template <class T, int Beg, int End>
		inline constexpr std::string_view get_enum_name(T n) {
			std::string_view s;
			static_for<Beg, End + 1>(get_enum_name_functor<T>((int)n, s));
			if (s.empty()) { return ""; }

	#if defined(_MSC_VER)
			size_t pos = s.find(',');
			pos += 1;
			size_t pos2 = s.find('>', pos);
	#else
			size_t pos = s.find("N = ");
			pos += 4;
			size_t pos2 = s.find_first_of(";]", pos);
	#endif
			s			= s.substr(pos, pos2 - pos);
			size_t pos3 = s.find("::");
			if (pos3 != std::string_view::npos) { s = s.substr(pos3 + 2); }
			return s;
		}

		//! Get the name of enum about concrete object
		//! \tparam T The type of the object.
		//! \param n The object to be detected.
		//! \return The name of object.
		template <class T>
		inline std::string get_enum_name(T n) {
			return std::string(get_enum_name<T, 0, 256>(n));
		}

		//! Get instance of specific type of enum by its name.
		//! \tparam T The type of enum.
		//! \tparam Beg The begin number to begin searching.
		//! \tparam End The end number to end searching.
		//! \param s The name of specific enum object.
		//! \return The instance of specific instance.
		template <class T, int Beg, int End>
		inline constexpr T enum_from_name(const std::string_view &s) {
			for (int i = Beg; i < End; i++) {
				if (s == get_enum_name((T)i)) { return (T)i; }
			}
			throw;
		}

		//! Get instance of specific type of enum by its name.
		//! \tparam T The type of enum.
		//! \param s The name of specific enum object.
		//! \return The instance of specific instance.
		template <class T>
		inline constexpr T enum_from_name(const std::string_view &s) {
			return enum_from_name<T, 0, 256>(s);
		}
	}

}

#endif //REFLECTION_ENUM_H
