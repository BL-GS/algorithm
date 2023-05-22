/*
 * @author: BL-GS 
 * @date:   2023/5/3
 */

#pragma once
#ifndef ALGORITHM_STRUCTURE_ARRAY_H
#define ALGORITHM_STRUCTURE_ARRAY_H

#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <type_traits>
#include <algorithm>

#include <iterator/random_iterator.h>

namespace algorithm::structure {

    inline namespace array {

		namespace detail {

			template<class Value>
			struct Iterator: iterator::RandomIteratorCRTP<Iterator<Value>, Value> {
			public:
				using ValueType     = std::remove_reference_t<Value>;
				using ReferenceType = ValueType &;
				using PointerType   = ValueType *;

			private:
				ValueType *ptr_;

			public:
				Iterator() = default;

				Iterator(ValueType *ptr): ptr_(ptr) {}

			public:
				ReferenceType dereference() const { return *ptr_; }

			public:
				void increment(ssize_t step = 1) { ptr_ += step; }

				void decrement(ssize_t step = 1) { ptr_ -= step; }

			public:
				bool equal(const Iterator &other) const { return ptr_ == other.ptr_; }

				auto compare(const Iterator &other) const { return ptr_ <=> other.ptr_; }
			};

			template<class Value>
			struct ReverseIterator: iterator::RandomIteratorCRTP<Iterator<Value>, Value> {
			public:
				using ValueType     = std::remove_reference_t<Value>;
				using ReferenceType = ValueType &;
				using PointerType   = ValueType *;

			private:
				ValueType *ptr_;

			public:
				ReverseIterator() = default;

				ReverseIterator(ValueType *ptr): ptr_(ptr) {}

			public:
				ReferenceType dereference() const { return *ptr_; }

			public:
				void increment(ssize_t step = 1) { ptr_ -= step; }

				void decrement(ssize_t step = 1) { ptr_ += step; }

			public:
				bool equal(const ReverseIterator &other) { return ptr_ == other.ptr_; }

				auto compare(const ReverseIterator &other) { return other.ptr_ <=> ptr_; }
			};
		}

		template<class Value, size_t Len>
		class Array {
		public:
            static_assert(Len > 0, "The length of array should be larger than 0.");

            using Self = Array<Value, Len>;

            using ValueType     = std::remove_reference_t<Value>;
            using ReferenceType = ValueType &;
            using PointerType   = ValueType *;

        public:
			using IteratorType             = detail::Iterator<ValueType>;
			using ConstIteratorType        = detail::ReverseIterator<const ValueType>;
			using ReverseIteratorType      = detail::Iterator<ValueType>;
			using ReverseConstIteratorType = detail::ReverseIterator<const ValueType>;

		private:
			ValueType value_array_[Len];

		public:
            Array() = default;

			Array(const Array &other) = default;

			Array(Array &&other) = default;

			Array(const Value &fill_value) {
                std::fill(value_array_, value_array_ + Len, fill_value);
            }

            Array(std::initializer_list<ValueType> &initializer_list) {
                for (size_t idx = 0; const ValueType &value: initializer_list) {
                    value_array_[idx] = value;
                    ++idx;
                }
            }

            ~Array() = default;

        public:
            ValueType &operator[] (size_t idx) {
                return value_array_[idx];
            }

        public:
            ValueType &get(size_t idx) {
                return value_array_[idx];
            }

            static consteval ssize_t size() {
                return Len;
            }

        public:
            IteratorType begin() const {
                return IteratorType{ value_array_ };
            }

			ConstIteratorType cbegin() const {
				return IteratorType{value_array_};
			}

			ReverseIteratorType rbegin() const {
				return IteratorType{value_array_};
			}

			ReverseConstIteratorType rcbegin() const {
				return IteratorType{value_array_};
			}

			IteratorType end() const {
				return IteratorType{value_array_ + Len};
			}

			ConstIteratorType cend() const {
				return IteratorType{value_array_ + Len};
			}

			ReverseIteratorType rend() const {
				return IteratorType{value_array_ + Len};
			}

			ReverseConstIteratorType rcend() const {
				return IteratorType{value_array_ + Len};
			}
		};
	}

}

#endif
