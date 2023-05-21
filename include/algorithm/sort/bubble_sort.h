/*
 * @author: BL-GS 
 * @date:   2023/5/15
 */

#pragma once
#ifndef ALGORITHM_ALGORITHM_SORT_BUBBLE_SORT_H
#define ALGORITHM_ALGORITHM_SORT_BUBBLE_SORT_H

#include <iterator>

namespace algorithm {

	inline namespace sort {

		template<class Iterator, class Cmp>
		inline void bubble_sort(Iterator start_iter, Iterator end_iter, Cmp cmp_func) {
			while (end_iter != start_iter) {
				bool do_swap = false;

				Iterator fast_iter = start_iter;
				Iterator slow_iter = fast_iter;

				while (true) {
					++fast_iter;
					if (fast_iter == end_iter) { break; }

					if (cmp_func(*slow_iter, *fast_iter)) {
						std::swap(*slow_iter, *fast_iter);
						do_swap = true;
					}
					slow_iter = fast_iter;
				}
				end_iter = slow_iter;

				if (!do_swap) { break; }
			}
		}

		template<class Iterator>
		inline void bubble_sort(Iterator start_iter, Iterator end_iter) {
			constexpr std::greater<decltype(*start_iter)> cmp_func;
			bubble_sort(start_iter, end_iter, cmp_func);
		}

	}

}

#endif//ALGORITHM_ALGORITHM_SORT_BUBBLE_SORT_H
