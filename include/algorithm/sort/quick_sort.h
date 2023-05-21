/*
 * @author: BL-GS 
 * @date:   2023/5/15
 */

#pragma once
#ifndef ALGORITHM_ALGORITHM_SORT_QUICK_SORT_H
#define ALGORITHM_ALGORITHM_SORT_QUICK_SORT_H

#include <iterator>

namespace algorithm {

	inline namespace sort {

		template<class Iterator, class Cmp>
		inline Iterator quick_sort_pivot(Iterator start_iter, Iterator end_iter, Cmp cmp_func) {
			Iterator last_iter = end_iter; --last_iter;

			if (last_iter == start_iter) { return start_iter; }

			Iterator pivot_iter = start_iter; ++start_iter;
			if (cmp_func(*start_iter, *pivot_iter)) {
				std::swap(*pivot_iter, *start_iter);
			}

			while (true) {
				while (start_iter != last_iter && cmp_func(*pivot_iter, *last_iter)) {
					--last_iter;
				}
				if (start_iter == last_iter) { break; }

				std::swap(*start_iter, *last_iter);

				while (start_iter != last_iter && !cmp_func(*pivot_iter, *start_iter)) {
					++start_iter;
				}
				if (start_iter == last_iter) { break; }

				std::swap(*start_iter, *last_iter);
			}

			return last_iter;
		}

		template<class Iterator, class Cmp>
		inline void quick_sort(Iterator start_iter, Iterator end_iter, Cmp cmp_func) {
			while (true) {
				size_t count = end_iter - start_iter;
				if (count <= 1) { break; }
				
				Iterator mid_iter = quick_sort_pivot(start_iter, end_iter, cmp_func);
				// Recursive resolution
				quick_sort(mid_iter, end_iter, cmp_func);
				// Iterative resolution
				end_iter = mid_iter;
			}
		}

		template<class Iterator>
		inline void quick_sort(Iterator start_iter, Iterator end_iter) {
			constexpr std::less<decltype(*start_iter)> cmp_func;
			quick_sort(start_iter, end_iter, cmp_func);
		}

	}
}

#endif//ALGORITHM_ALGORITHM_SORT_QUICK_SORT_H
