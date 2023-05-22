/*
 * @author: BL-GS 
 * @date:   2023/5/3
 */

#pragma once
#ifndef ALGORITHM_ITERATOR_BILATERAL_ITERATOR_H
#define ALGORITHM_ITERATOR_BILATERAL_ITERATOR_H

#include <iterator/iterator_type.h>

namespace algorithm::iterator {

	namespace detail {
		template<class Impl>
		concept BilateralIterImplConcept =  requires(
		        Impl impl,
		        ssize_t step) {

			{ impl.increment() };

			{ impl.decrement() };

			{ impl.equal(impl) } -> std::same_as<bool>;
		};
	}

	template <class Impl, class Value>
	class IteratorCRTP<Impl, Value, IteratorType::Bilateral> {
	public:
		using ValueType        = Value;
		using ReferenceType    = ValueType&;
		using PointerType      = ValueType*;
		using DifferenceType   = ssize_t;

		static constexpr IteratorType IteratorCategory = IteratorType::Bilateral;

	public:
		friend bool operator==(const Impl& lhs, const Impl& rhs) { return equal(lhs, rhs); }

		friend bool operator!=(const Impl& lhs, const Impl& rhs) { return !equal(lhs, rhs); }

		Value & operator*() const { return as_derived_const().dereference(); }

		Value * operator->() const { return std::addressof(operator*()); }

		Impl & operator++() {
			as_derived().increment();
			return as_derived();
		}

		Impl operator++(int) {
			auto ret = as_derived(); // copy
			as_derived().increment();
			return ret;
		}

		Impl & operator--() {
			as_derived().decrement();
			return as_derived();
		}

		Impl operator--(int) {
			auto ret = as_derived(); // copy
			as_derived().decrement();
			return ret;
		}

	private:
		Impl &as_derived() { return static_cast<Impl &>(*this); }

		const Impl&as_derived_const() const { return static_cast<const Impl&>(*this); }

		static bool equal(const Impl& lhs, const Impl& rhs) { return lhs.equal(rhs); }
	};
}

#endif//ALGORITHM_ITERATOR_BILATERAL_ITERATOR_H