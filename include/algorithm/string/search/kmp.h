/*
 * @author: BL-GS 
 * @date:   2023/5/17
 */

#pragma once
#ifndef ALGORITHM_ALGORITHM_STRING_KMP_H
#define ALGORITHM_ALGORITHM_STRING_KMP_H

#include <cstddef>
#include <cstring>
#include <limits>
#include <span>
#include <vector>

#include <algorithm/string/search/base.h>

namespace algorithm {

	inline namespace string {

		template<class Value>
		class KMP {
		public:
			using ValueType = Value;

		private:
			size_t *dp_array_;
			/// The size of pattern
			size_t pattern_size_;
			/// The amount of kinds of value
			size_t value_amount_;
			/// The minimal value
			ValueType min_value_;

		public:
			template<class Iterator>
			KMP(Iterator start_iter, Iterator end_iter) : dp_array_(nullptr), pattern_size_(end_iter - start_iter),
			                                              value_amount_(std::numeric_limits<ValueType>::max() - std::numeric_limits<ValueType>::min() + 1),
			                                              min_value_(std::numeric_limits<ValueType>::min()) {

				size_t total_size = pattern_size_ * value_amount_;
				dp_array_ = new size_t[total_size];

				build_dp_table({start_iter, end_iter}, dp_array_, pattern_size_, value_amount_);
			}

			template<class Iterator>
			KMP(Iterator start_iter, Iterator end_iter,
			    const ValueType &min_value, const ValueType &max_value) : dp_array_(nullptr), pattern_size_(end_iter - start_iter),
			                                                                                                      value_amount_(max_value - min_value + 1), min_value_(min_value) {

				size_t total_size = pattern_size_ * value_amount_;
				dp_array_ = new size_t[total_size];

				build_dp_table({start_iter, end_iter}, dp_array_, pattern_size_, value_amount_);
			}

			~KMP() noexcept {
				delete[] dp_array_;
			}

		public:
			template<class Iterator>
			Iterator find(Iterator start_iter, Iterator end_iter) {
				size_t total_size = pattern_size_ * value_amount_;
				return find(start_iter, end_iter, {dp_array_, total_size}, pattern_size_);
			}

			template<class Iterator>
			std::vector<Iterator> find_all(Iterator start_iter, Iterator end_iter) {
				size_t total_size = pattern_size_ * value_amount_;
				return find_all(start_iter, end_iter, {dp_array_, total_size}, pattern_size_);
			}

		private:
			constexpr size_t get_dp_idx(size_t pattern_idx, const ValueType &pattern_value) {
				return get_dp_idx(pattern_idx, pattern_value, value_amount_, min_value_);
			}

		public:
			static constexpr size_t get_dp_idx(size_t pattern_idx, const ValueType &pattern_value,
			                                   size_t value_amount, const ValueType &min_value) {
				return pattern_idx * value_amount + (pattern_value - min_value);
			}

			static constexpr size_t get_dp_idx(size_t pattern_idx, size_t pattern_value_idx,
			                                   size_t value_amount) {
				return pattern_idx * value_amount + pattern_value_idx;
			}

			static constexpr void build_dp_table(std::span<ValueType> pattern, std::span<size_t> dp_array,
			                                     size_t pattern_size, size_t value_amount,
			                                     const ValueType &min_value) {
				auto inner_dp = [&](size_t pattern_idx, const ValueType &pattern_value) -> std::add_lvalue_reference_t<size_t> {
					return dp_array[get_dp_idx(pattern_idx, pattern_value, value_amount, min_value)];
				};

				auto inner_dp_direct = [&](size_t pattern_idx, size_t pattern_value_idx) -> std::add_lvalue_reference_t<size_t> {
					return dp_array[get_dp_idx(pattern_idx, pattern_value_idx, value_amount)];
				};

				size_t total_size = pattern_size * value_amount;
				assert(dp_array.size() >= total_size);

				// Calculate DP array
				std::memset(dp_array.data(), 0, total_size * sizeof(size_t));
				inner_dp(0, pattern[0]) = 1;

				size_t shadow_idx = 0;
				for (size_t i = 1; i < pattern_size; ++i) {
					for (size_t k = 0; k < value_amount; ++k) {
						inner_dp_direct(i, k) = inner_dp_direct(shadow_idx, k);
					}
					inner_dp(i, pattern[i]) = i + 1;
					shadow_idx = inner_dp(shadow_idx, pattern[i]);
				}
			}

			template<class Iterator>
			static constexpr Iterator find(Iterator start_iter, Iterator end_iter,
			                               std::span<size_t> dp_array,
			                               size_t pattern_size, size_t value_amount,
			                               const ValueType &min_value) {
				// Find sub string
				size_t pattern_iter_pos = 0;
				for (Iterator iter = start_iter; iter != end_iter; ++iter) {
					size_t pos = iter - start_iter;
					// State transition
					pattern_iter_pos = dp_array[get_dp_idx(pattern_iter_pos, *iter, value_amount, min_value)];

					if (pattern_iter_pos == pattern_size) {// If the pattern is entirely matched.
						// Return the result
						return iter - pattern_size + 1;
					}
				}
				// No match
				return end_iter;
			}

			template<class Iterator>
			static constexpr std::vector<Iterator> find_all(Iterator start_iter, Iterator end_iter,
			                                                std::span<size_t> dp_array,
			                                                size_t pattern_size, size_t value_amount,
			                                                const ValueType &min_value) {
				std::vector<Iterator> res_iters;
				// Find sub string
				size_t pattern_iter_pos = 0;
				for (Iterator iter = start_iter; iter != end_iter; ++iter) {
					// State transition
					pattern_iter_pos = dp_array[get_dp_idx(pattern_iter_pos, *iter, value_amount, min_value)];

					if (pattern_iter_pos == pattern_size) {// If the pattern is entirely matched.
						// Add a result
						Iterator res_iter = iter - pattern_size + 1;
						res_iters.emplace_back(res_iter);
						// Reset scanner
						iter = res_iters + 1;
						pattern_iter_pos = 0;
					}
				}
				return res_iters;
			}
		};

		template<class Value, class VIterator, class Iterator>
		inline Iterator kmp_find(VIterator value_start_iter, VIterator value_end_iter,
		                         Iterator start_iter, Iterator end_iter,
		                         const Value &min_value, const Value &max_value) {

			using PatternValueType = std::remove_reference_t<decltype(*value_start_iter)>;
			using BaseValueType    = std::remove_reference_t<decltype(*start_iter)>;
			using KMPStrategy      = KMP<Value>;

			constexpr size_t STACK_ARRAY_LIMIT = 512;

			std::span<PatternValueType> pattern{value_start_iter, value_end_iter};

			size_t pattern_size = pattern.size();
			size_t value_amount = max_value - min_value + 1;
			size_t total_size   = pattern_size * value_amount;

			if (total_size > STACK_ARRAY_LIMIT) {
				size_t *dp_array = new size_t[total_size];

				// Calculate DP array
				KMPStrategy::build_dp_table(pattern, {dp_array, total_size},
				                            pattern_size, value_amount,
				                            min_value);
				// Find sub string
				auto res_iter = KMPStrategy::find(start_iter, end_iter,
				                                  {dp_array, total_size},
				                                  pattern_size, value_amount,
				                                  min_value);

				delete[] dp_array;
				return res_iter;
			}
			else {
				size_t dp_array[total_size];

				// Calculate DP array
				KMPStrategy::build_dp_table(pattern, {dp_array, total_size},
				                            pattern_size, value_amount,
				                            min_value);
				// Find sub string
				auto res_iter = KMPStrategy::find(start_iter, end_iter,
				                                  {dp_array, total_size},
				                                  pattern_size, value_amount,
				                                  min_value);

				return res_iter;
			}

			return end_iter;
		}

		template<class VIterator, class Iterator>
		Iterator kmp_find(VIterator value_start_iter, VIterator value_end_iter,
		                  Iterator start_iter, Iterator end_iter) {
			using ValueType    = std::remove_reference_t<decltype(*start_iter)>;
			return kmp_find(
			        value_start_iter, value_end_iter,
			        start_iter, end_iter,
			        std::numeric_limits<ValueType>::min(),
			        std::numeric_limits<ValueType>::max()
			);
		}

	}

}

#endif//ALGORITHM_ALGORITHM_STRING_KMP_H
