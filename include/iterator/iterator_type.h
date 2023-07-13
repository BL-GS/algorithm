/*
 * @author: BL-GS 
 * @date:   2023/5/22
 */

#pragma once
#ifndef ALGORITHM_ITERATOR_ITEARTOR_TYPE_H
#define ALGORITHM_ITERATOR_ITEARTOR_TYPE_H

#include <concepts>
#include <cstddef>
#include <cstdint>

namespace algorithm::iterator {

	enum class IteratorType {
		Abstract,
		Forwarding,
		Bilateral,
		Random
	};

	template <class Impl, class Value, IteratorType Tag>
	class IteratorCRTP {};

	template <class Impl, class Value>
	using AbstractIteratorCRTP = IteratorCRTP<Impl, Value, IteratorType::Abstract>;

	template <class Impl, class Value>
	using ForwardingIteratorCRTP = IteratorCRTP<Impl, Value, IteratorType::Forwarding>;

	template <class Impl, class Value>
	using BilateralIteratorCRTP = IteratorCRTP<Impl, Value, IteratorType::Bilateral>;

	template <class Impl, class Value>
	using RandomIteratorCRTP = IteratorCRTP<Impl, Value, IteratorType::Random>;

	template<class Iter>
	concept AbstractIterConcept = requires(
	        Iter iter) {

		typename Iter::ValueType;

		typename Iter::ReferenceType;

		typename Iter::PointerType;

		typename Iter::DifferenceType;

		{ *iter } -> std::same_as<typename Iter::ReferenceType>;

		{ iter.operator->() } -> std::same_as<typename Iter::PointerType>;

		{ iter == iter } -> std::same_as<bool>;

		{ iter != iter } -> std::same_as<bool>;
	};

	template<class ForwardingIter>
	concept ForwardingIterConcept = requires(ForwardingIter iter) {

		requires AbstractIterConcept<ForwardingIter>;

		{ iter++ } -> std::same_as<ForwardingIter>;

		{ ++iter } -> std::same_as<ForwardingIter &>;
	};

	template<class BilateralIter>
	concept BilateralIterConcept = requires(BilateralIter iter) {
		requires ForwardingIterConcept<BilateralIter>;

		{ iter-- } -> std::same_as<BilateralIter>;

		{ --iter } -> std::same_as<BilateralIter &>;
	};

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

#endif//ALGORITHM_ITERATOR_ITEARTOR_TYPE_H
