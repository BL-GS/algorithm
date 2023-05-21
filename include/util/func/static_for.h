/*
 * @author: BL-GS 
 * @date:   2023/5/14
 */

#pragma once
#ifndef ALGORITHM_UTIL_STATIC_FOR_H
#define ALGORITHM_UTIL_STATIC_FOR_H

namespace algorithm::util {

	inline namespace func {

		template<class Func, int Beg, int End>
		concept StaticForFuncConcept = requires(Func f) {
			{ f.template call<Beg>() };
			{ f.template call<End>() };
		};

		//! For loop at compilation time.
		//! \tparam Beg The begin of loop counter
		//! \tparam End The end of loop counter
		//! \tparam F The type of function to be called
		//! \param func The function to be called each time.
		//! \return
		template <int Beg, int End, class F>
			requires (Beg == End) && StaticForFuncConcept<F, Beg, End>
		inline void static_for(const F &func) {}

		//! For loop at compilation time.
		//! \tparam Beg The begin of loop counter
		//! \tparam End The end of loop counter
		//! \tparam F The type of function to be called
		//! \param func The function to be called each time.
		//! \return
		template <int Beg, int End, class F>
			requires (Beg != End) && StaticForFuncConcept<F, Beg, End>
		inline void static_for(const F &func) {
			func.template call<Beg>();
			static_for<Beg + 1, End>(func);
		}

	}
}

#endif//ALGORITHM_UTIL_STATIC_FOR_H
