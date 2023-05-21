/*
 * @author: BL-GS 
 * @date:   2023/5/3
 */

#pragma once
#ifndef ALGORITHM_ITERATOR_BILATERAL_ITERATOR_H
#define ALGORITHM_ITERATOR_BILATERAL_ITERATOR_H

#include <concepts>
#include <iterator/forwarding_iterator.h>

namespace algorithm::iterator {

	template<class BilateralIter>
	concept BilateralIterConcept = requires(BilateralIter iter) {
		requires ForwardingIterConcept<BilateralIter>;

        { iter-- } -> std::same_as<BilateralIter>;

        { --iter } -> std::same_as<BilateralIter &>;
	};
}

#endif//ALGORITHM_ITERATOR_BILATERAL_ITERATOR_H