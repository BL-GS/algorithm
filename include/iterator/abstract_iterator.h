/*
 * @author: BL-GS 
 * @date:   2023/5/3
 */

#pragma once
#include <functional>
#ifndef ALGORITHM_ITERATOR_ABSTRACT_ITERATOR_H
#define ALGORITHM_ITERATOR_ABSTRACT_ITERATOR_H

#include <iterator/iterator_type.h>

namespace algorithm::iterator {

	namespace detail {
		template<class Impl>
		concept AbstractIterImplConcept =  requires(
		        Impl impl,
		        ssize_t step) {
			{ impl.equal(impl) } -> std::same_as<bool>;
		};
	}

	template <class Impl, class Value>
	class IteratorCRTP<Impl, Value, IteratorType::Abstract> {
	public:
		using ValueType        = Value;
		using ReferenceType    = ValueType&;
		using PointerType      = ValueType*;
		using DifferenceType   = ssize_t;

		static constexpr IteratorType IteratorCategory = IteratorType::Abstract;

	public:
		friend bool operator==(const Impl& lhs, const Impl& rhs) { return equal(lhs, rhs); }

		friend bool operator!=(const Impl& lhs, const Impl& rhs) { return !equal(lhs, rhs); }

		Value & operator*() const { return as_derived_const().dereference(); }

		Value * operator->() const { return std::addressof(operator*()); }

	private:
		Impl &as_derived() { return static_cast<Impl &>(*this); }

		const Impl&as_derived_const() const { return static_cast<const Impl&>(*this); }

		static bool equal(const Impl& lhs, const Impl& rhs) { return lhs.equal(rhs); }
	};

}


#endif//ALGORITHM_ITERATOR_ABSTRACT_ITERATOR_H