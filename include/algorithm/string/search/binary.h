/*
 * @author: BL-GS 
 * @date:   2023/5/17
 */

#pragma once
#ifndef ALGORITHM_ALGORITHM_STRING_BINARY_H
#define ALGORITHM_ALGORITHM_STRING_BINARY_H

#include <cstddef>
#include <limits>

#include <algorithm/string/search/base.h>

namespace algorithm {

	inline namespace string {

		template<class V, class Iterator, class Cmp>
		constexpr size_t binary_find_idx(const V &value, Iterator start_iter, Iterator end_iter, const Cmp &cmp_func) {
			size_t start_pos = 0;
			size_t end_pos   = end_iter - start_iter;

			while (start_pos != end_pos) {
				size_t count = end_pos - start_pos;
				size_t pos   = count / 2;

				Iterator cur_iter = start_iter + pos;
				if (*cur_iter == value) { return pos; }

				if (cmp_func(*cur_iter, value)) {
					start_pos = pos + 1;
				}
				else {
					end_pos = pos;
				}
			}
			return STRING_NO_FOUND_POS;
		}

		template<class V, class Iterator, class Cmp>
		constexpr Iterator binary_find(const V &value, Iterator start_iter, Iterator end_iter, const Cmp &cmp_func) {
			size_t start_pos = 0;
			size_t end_pos   = end_iter - start_iter;

			while (start_pos != end_pos) {
				size_t count = end_pos - start_pos;
				size_t pos   = count / 2;

				Iterator cur_iter = start_iter + pos;
				if (*cur_iter == value) { return cur_iter; }

				if (cmp_func(*cur_iter, value)) {
					start_pos = pos + 1;
				}
				else {
					end_pos = pos;
				}
			}
			return end_iter;
		}

	}

}

#endif//ALGORITHM_ALGORITHM_STRING_BINARY_H
