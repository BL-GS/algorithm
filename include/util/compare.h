/*
 * @author: BL-GS 
 * @date:   2023/5/11
 */

#pragma once
#ifndef ALGORITHM_UTIL_COMPARE_H
#define ALGORITHM_UTIL_COMPARE_H

#include <iterator>

namespace algorithm::util {

	inline namespace func {

		template<class Iterator, class Cmp>
		inline Iterator argidx(Iterator start_iter, Iterator end_iter, Cmp cmp_func) {
			Iterator res_iter = start_iter;

			auto iter = start_iter;
			for (++iter; iter != end_iter; ++iter) {
				if (cmp_func(*iter, *res_iter)) {
					res_iter = iter;
				}
			}
			return res_iter;
		}

		template<class Iterator>
		inline Iterator argmin(Iterator start_iter, Iterator end_iter) {
			constexpr std::less<decltype(*start_iter)> cmp_func;
			return argidx(start_iter, end_iter, cmp_func);
		}

		template<class Iterator>
		inline Iterator argmax(Iterator start_iter, Iterator end_iter) {
			constexpr std::greater<decltype(*start_iter)> cmp_func;
			return argidx(start_iter, end_iter, cmp_func);
		}



	}

}

#endif//ALGORITHM_UTIL_COMPARE_H
