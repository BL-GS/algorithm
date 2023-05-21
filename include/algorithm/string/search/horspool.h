/*
 * @author: BL-GS 
 * @date:   2023/5/17
 */

#pragma once
#ifndef ALGORITHM_ALGORITHM_STRING_HORSPOOL_H
#define ALGORITHM_ALGORITHM_STRING_HORSPOOL_H

#include <cstddef>
#include <cstring>
#include <limits>
#include <span>

#include <algorithm/string/search/base.h>

namespace algorithm {

	inline namespace string {

		template<class Value>
		void build_bad_character_heuristic(std::span<Value> pattern, std::span<ssize_t> bad_character_heuristic_array,
		                                   const Value &min_value) {
			ssize_t pattern_size = pattern.size();
			std::fill(bad_character_heuristic_array.begin(), bad_character_heuristic_array.end(), -1);
			for (ssize_t i = 0; i < pattern_size - 1; ++i) {
				bad_character_heuristic_array[pattern[i] - min_value] = i;
			}
		}

		template<class Value, class VIterator, class Iterator>
		Iterator boyer_moore_find(VIterator value_start_iter, VIterator value_end_iter,
		                          Iterator start_iter, Iterator end_iter,
		                          const Value &min_value, const Value &max_value) {

			using PatternValueType = std::remove_reference_t<decltype(*value_start_iter)>;
			using BaseValueType = std::remove_reference_t<decltype(*start_iter)>;

			std::span<PatternValueType> pattern{value_start_iter, value_end_iter};
			size_t pattern_size = pattern.size();
			size_t value_range = max_value - min_value + 1;

			ssize_t *bc_heuristic = new ssize_t[value_range];

			std::span<ssize_t> bc_array{bc_heuristic, value_range};

			build_bad_character_heuristic(pattern, bc_array, min_value);

			std::span<BaseValueType> base{start_iter, end_iter};
			ssize_t base_size = base.size();

			ssize_t base_idx = 0;
			while (base_idx <= base_size - pattern_size) {
				ssize_t pattern_idx = pattern_size - 1;

				while (base[base_idx + pattern_idx] == pattern[pattern_idx]) {
					if (--pattern_idx < 0) {
						delete[] bc_heuristic;
						return start_iter + base_idx;
					}
				}
				base_idx += pattern_idx - bc_heuristic[base[base_idx + pattern_idx] - min_value];
			}

			delete[] bc_heuristic;
			return end_iter;
		}

		template<class VIterator, class Iterator>
		Iterator boyer_moore_find(VIterator value_start_iter, VIterator value_end_iter,
		                          Iterator start_iter, Iterator end_iter) {
			using ValueType = std::remove_reference_t<decltype(*start_iter)>;
			return boyer_moore_find(value_start_iter, value_end_iter,
			                        start_iter, end_iter,
			                        std::numeric_limits<ValueType>::min(),
			                        std::numeric_limits<ValueType>::max());
		}
	}

}

#endif//UTIL_ALGORITHM_HORSPOOL_H
