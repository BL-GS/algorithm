/*
* @author: BL-GS
* @date:   2023/5/15
*/

#pragma once
#ifndef ALGORITHM_ALGORITHM_SEARCH_BINARY_SEARCH_H
#define ALGORITHM_ALGORITHM_SEARCH_BINARY_SEARCH_H

#include <cstddef>
#include <cstdint>
#include <bit>

namespace algorithm::search {

	/*!
	 * @brief Find the lower bound of specific value in sorted array. (Binary Search)
	 * @param begin The begin iterator
	 * @param end The end iterator
	 * @param value The expected value
	 * @param comp The compare function
	 * @return The lower bound coordinating to value
	 */
	template<typename Iterator, typename T, typename Cmp>
	Iterator lower_bound(Iterator begin, Iterator end, const T& value, Cmp comp) {
		size_t n = end - begin;
		size_t b = 0;
		for (size_t bit = std::bit_floor(n); bit != 0; bit >>= 1) {
			size_t i = (b | bit) - 1;
			if (i < n && comp(*(begin + i), value)) { b |= bit; }
		}
		return begin + b;
	}

}

#endif//ALGORITHM_ALGORITHM_SEARCH_BINARY_SEARCH_H