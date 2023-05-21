/*
 * @author: BL-GS 
 * @date:   2023/5/3
 */

#pragma once
#ifndef ALGORITHM_ITERATOR_RANDOM_ITERATOR_H
#define ALGORITHM_ITERATOR_RANDOM_ITERATOR_H

#include <compare>
#include <concepts>
#include <iterator/bilateral_iterator.h>

namespace algorithm::iterator {

	template<class RandomIter>
	concept RandomIterConcept = requires(RandomIter iter) {
		requires BilateralIterConcept<RandomIter>;

		{ iter + 2 } -> std::same_as<RandomIter>;

		{ iter - 2 } -> std::same_as<RandomIter>;

		{ iter += 2 } -> std::same_as<void>;

		{ iter -= 2 } -> std::same_as<void>;

		{ iter - iter } -> std::same_as<std::size_t>;

		{ iter <= iter } -> std::same_as<bool>;

		{ iter >= iter } -> std::same_as<bool>;

		{ iter <  iter } -> std::same_as<bool>;

		{ iter >  iter } -> std::same_as<bool>;
	};
}

#endif//ALGORITHM_ITERATOR_RANDOM_ITERATOR_H