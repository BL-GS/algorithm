/*
 * @author: BL-GS 
 * @date:   2023/5/17
 */

#pragma once
#ifndef ALGORITHM_ALGORITHM_STRING_BOYER_MOORE_H
#define ALGORITHM_ALGORITHM_STRING_BOYER_MOORE_H

#include <cstddef>
#include <cstring>
#include <limits>
#include <span>

#include <algorithm/string/search/base.h>

namespace algorithm {

	inline namespace string {

		template<class Value>
		class BoyerMoore {
		public:
			using ValueType = Value;

		private:
			std::span<ssize_t> bad_character_heuristic_;

			std::span<ssize_t> good_suffix_heuristic_;

			std::span<ValueType> pattern_;

			ValueType min_value_;

		public:
			template<class Iterator>
			BoyerMoore(Iterator start_iter, Iterator end_iter) {
				this(start_iter, end_iter, std::numeric_limits<ValueType>::min(), std::numeric_limits<ValueType>::max());
			}

			template<class Iterator>
			BoyerMoore(Iterator start_iter, Iterator end_iter, const ValueType &min_value, const ValueType &max_value): min_value_(min_value) {
		        using PatternValueType = std::remove_reference_t<decltype(*start_iter)>;

				std::span<PatternValueType> pattern{start_iter, end_iter};
				size_t pattern_size = pattern.size();
				size_t value_range = max_value - min_value + 1;

				bad_character_heuristic_ = std::span<ssize_t>{new ssize_t[value_range], value_range};
				good_suffix_heuristic_ = std::span<ssize_t>{new ssize_t[pattern_size], pattern_size};

				// Build two tables of heuristic
				build_bad_character_heuristic(pattern, bad_character_heuristic_, min_value);
				build_good_suffix_heuristic(pattern, good_suffix_heuristic_);

				// Copy the pattern
				pattern_ = {new ValueType[pattern_size], pattern_size};
				std::copy(start_iter, end_iter, pattern_.begin());
			}

			~BoyerMoore() {
				delete[] bad_character_heuristic_.data();
				delete[] good_suffix_heuristic_.data();
				delete[] pattern_.data();
			}

		public:
			template<class Iterator>
			Iterator find(Iterator start_iter, Iterator end_iter) {
				return find(start_iter, end_iter,
				            pattern_,
				            bad_character_heuristic_,
				            good_suffix_heuristic_,
				            min_value_);
			}

			template<class Iterator>
			std::vector<Iterator> find_all(Iterator start_iter, Iterator end_iter) {
				return find_all(start_iter, end_iter,
				                pattern_,
				                bad_character_heuristic_,
				                good_suffix_heuristic_,
				                min_value_);
			}

		public:
			static constexpr void build_suffix_length_array(std::span<ValueType> pattern,
			                                                std::span<ssize_t> suffix_length_array) {
				ssize_t pattern_size = pattern.size();

				ssize_t upper = pattern_size - 1;
				ssize_t lower = upper;

				suffix_length_array[pattern_size - 1] = pattern_size;
				for (ssize_t cur_pos = upper - 1; cur_pos >= 0; --cur_pos) {
					if (cur_pos > lower && suffix_length_array[pattern_size - 1 - (upper - cur_pos)] <= cur_pos - lower) {
						suffix_length_array[cur_pos] = suffix_length_array[pattern_size - 1 - (upper - cur_pos)];
					}
					else {
						upper = cur_pos;
						lower = std::min(lower, upper);

						while (lower >= 0 && pattern[lower] == pattern[pattern_size - 1 - (upper - lower)]) {
							--lower;
						}
						suffix_length_array[cur_pos] = upper - lower;
					}
				}
			}

			static constexpr void build_bad_character_heuristic(std::span<ValueType> pattern,
			                                                    std::span<ssize_t> bad_character_heuristic_array,
			                                                    const Value &min_value) {
				ssize_t pattern_size = pattern.size();
				std::fill(bad_character_heuristic_array.begin(), bad_character_heuristic_array.end(), -1);
				for (ssize_t i = 0; i < pattern_size - 1; ++i) {
					bad_character_heuristic_array[pattern[i] - min_value] = i;
				}
			}

			static constexpr void build_good_suffix_heuristic(std::span<ValueType> pattern,
			                                                  std::span<ssize_t> good_suffix_heuristic_array) {
				ssize_t pattern_size = pattern.size();
				std::fill(good_suffix_heuristic_array.begin(), good_suffix_heuristic_array.end(), pattern_size);

				ssize_t upper = pattern_size - 1;
				ssize_t lower = upper;

				ssize_t *suffix_length_array = new ssize_t[pattern_size];
				std::span<ssize_t> suffix_length_array_span{suffix_length_array, static_cast<size_t>(pattern_size)};
				build_suffix_length_array(pattern, suffix_length_array_span);

				std::fill(good_suffix_heuristic_array.begin(), good_suffix_heuristic_array.end(), pattern_size);
				for (ssize_t i = 0, j = pattern_size; j > 0; --j) {
					if (j == suffix_length_array_span[j - 1]) {
						while (i < pattern_size - j) {
							good_suffix_heuristic_array[i++] = pattern_size - j;
						}
					}
				}
				for (ssize_t i = 0; i < pattern_size - 1; ++i) {
					good_suffix_heuristic_array[pattern_size - 1 - suffix_length_array_span[i]] = pattern_size - 1 - i;
				}

				delete[] suffix_length_array;
			}

			template<class Iterator>
			static constexpr Iterator find(Iterator start_iter, Iterator end_iter,
			                               std::span<ValueType> pattern,
			                               std::span<ssize_t> bad_character_heuristic_array,
			                               std::span<ssize_t> good_suffix_heuristic_array,
			                               const ValueType &min_value) {
				using BaseValueType = std::remove_reference_t<decltype(*start_iter)>;

				std::span<BaseValueType> base{start_iter, end_iter};
				ssize_t base_size = base.size();
				ssize_t pattern_size = pattern.size();

				ssize_t base_idx = 0;
				while (base_idx <= base_size - pattern_size) {
					ssize_t pattern_idx = pattern_size - 1;

					while (base[base_idx + pattern_idx] == pattern[pattern_idx]) {
						if (--pattern_idx < 0) {
							return start_iter + base_idx;
						}
					}
					base_idx += std::max(good_suffix_heuristic_array[pattern_idx],
					                     pattern_idx - bad_character_heuristic_array[base[base_idx + pattern_idx] - min_value]);
				}
				return end_iter;
			}

			template<class Iterator>
			static constexpr Iterator find_all(Iterator start_iter, Iterator end_iter,
			                                   std::span<ValueType> pattern,
			                                   std::span<ssize_t> bad_character_heuristic_array,
			                                   std::span<ssize_t> good_suffix_heuristic_array,
			                                   const ValueType &min_value) {
				using BaseValueType = std::remove_reference_t<decltype(*start_iter)>;

				std::span<BaseValueType> base{start_iter, end_iter};
				ssize_t base_size = base.size();
				ssize_t pattern_size = pattern.size();

				std::vector<ValueType> res_iters;

				ssize_t base_idx = 0;
				while (base_idx <= base_size - pattern_size) {
					ssize_t pattern_idx = pattern_size - 1;

					bool found = false;
					while (base[base_idx + pattern_idx] == pattern[pattern_idx]) {
						if (--pattern_idx < 0) {
							res_iters.emplace_back(start_iter + base_idx);
							base_idx += 1;
							found = true;
						}
					}
					if (!found) {
						base_idx += std::max(good_suffix_heuristic_array[pattern_idx],
						                     pattern_idx - bad_character_heuristic_array[base[base_idx + pattern_idx] - min_value]);
					}
				}
			}
		};

		template<class Value, class VIterator, class Iterator>
		Iterator boyer_moore_find(VIterator value_start_iter, VIterator value_end_iter,
		                          Iterator start_iter, Iterator end_iter,
		                          const Value &min_value, const Value &max_value) {
	        BoyerMoore<Value> finder{value_start_iter, value_end_iter, min_value, max_value};
			return finder.find(start_iter, end_iter);
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

#endif//ALGORITHM_ALGORITHM_STRING_BOYER_MOORE_H
