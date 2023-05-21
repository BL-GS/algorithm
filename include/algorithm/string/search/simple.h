/*
 * @author: BL-GS 
 * @date:   2023/5/17
 */

#pragma once
#ifndef ALGORITHM_ALGORITHM_STRING_SIMPLE_H
#define ALGORITHM_ALGORITHM_STRING_SIMPLE_H

#include <cstddef>
#include <limits>

#include <algorithm/string/search/base.h>

namespace algorithm {

	inline namespace string {

		template<class V, class Iterator>
		constexpr size_t simple_find_idx(const V &value, Iterator start_iter, Iterator end_iter) {
			size_t pos = 0;
			for (Iterator iter = start_iter; iter != end_iter; ++iter, ++pos) {
				if (*iter == value) {
					return pos;
				}
			}
			return STRING_NO_FOUND_POS;
		}

		template<class VIterator, class Iterator>
		constexpr size_t simple_find_idx(VIterator value_start_iter, VIterator value_end_iter,
		                                 Iterator start_iter, Iterator end_iter) {
			size_t pos = 0;
			for (Iterator iter = start_iter; iter != end_iter; ++iter, ++pos) {
				if (*iter == *value_start_iter) {
					Iterator match_iter = iter;
					VIterator pattern_iter = value_start_iter;

					while (pattern_iter != value_end_iter) {
						if (*match_iter != *pattern_iter) { break; }
						++match_iter;
						++pattern_iter;
					}

					if (pattern_iter == value_end_iter) { return pos; }
				}
			}
			return STRING_NO_FOUND_POS;
		}

		template<class V, class Iterator>
		constexpr Iterator simple_find(const V &value, Iterator start_iter, Iterator end_iter) {
			for (Iterator iter = start_iter; iter != end_iter; ++iter) {
				if (*iter == value) {
					return iter;
				}
			}
			return end_iter;
		}

		template<class VIterator, class Iterator>
		constexpr Iterator simple_find(VIterator value_start_iter, VIterator value_end_iter,
		                               Iterator start_iter, Iterator end_iter) {
			for (Iterator iter = start_iter; iter != end_iter; ++iter) {
				if (*iter == *value_start_iter) {
					Iterator match_iter = iter;
					VIterator pattern_iter = value_start_iter;

					while (pattern_iter != value_end_iter) {
						if (*match_iter != *pattern_iter) { break; }
						++match_iter;
						++pattern_iter;
					}

					if (pattern_iter == value_end_iter) { return iter; }
				}
			}
			return end_iter;
		}


	}

}

#endif//ALGORITHM_ALGORITHM_STRING_SIMPLE_H
