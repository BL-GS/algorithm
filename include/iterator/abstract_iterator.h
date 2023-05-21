/*
 * @author: BL-GS 
 * @date:   2023/5/3
 */

#pragma once
#include <functional>
#ifndef ALGORITHM_ITERATOR_ABSTRACT_ITERATOR_H
#define ALGORITHM_ITERATOR_ABSTRACT_ITERATOR_H

#include <concepts>

namespace algorithm::iterator {

	template<class Iter>
	concept AbstractIterConcept = requires(
        Iter iter) {

		typename Iter::ValueType;

		typename Iter::ReferenceType;

        typename Iter::PointerType;
        
        { *iter } -> std::same_as<typename Iter::ReferenceType>;

        { iter.operator->() } -> std::same_as<typename Iter::PointerType>;

		{ iter == iter } -> std::same_as<bool>;
	};

}


#endif//ALGORITHM_ITERATOR_ABSTRACT_ITERATOR_H