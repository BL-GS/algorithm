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

        template<class Value>
		struct Iterator {
        public:
			using ValueType     = std::remove_reference_t<Value>;
			using ReferenceType = ValueType &;
			using PointerType   = ValueType *;

        private:
			ValueType *ptr_;

		public:
			Iterator &operator++() {
				ptr_++;
				return *this;
			}

			Iterator &operator--() {
				ptr_--;
				return *this;
			}

			Iterator operator++(int) {
				Iterator res = *this;
				ptr_++;
				return res;
			}

			Iterator operator--(int) {
				Iterator res = *this;
				ptr_--;
				return res;
			}

			Iterator operator+(size_t idx) const {
				return {.ptr_ = ptr_ + idx};
			}

			Iterator operator-(size_t idx) const {
				return {.ptr_ = ptr_ - idx};
			}

			void operator+=(size_t idx) {
				ptr_ += idx;
			}

			void operator-=(size_t idx) {
				ptr_ -= idx;
			}

			size_t operator- (const Iterator &other) const {
				return other.ptr_ - ptr_;
			}

			auto operator<=>(const Iterator &other) const {
				return ptr_ <=> other.ptr_;
			}

			bool operator==(const Iterator &other) const {
				return ptr_ == other.ptr_;
			}

            Value &operator*() {
                return *ptr_;
            }

            Value *operator->() {
                return ptr_;
            }
		};

		template<class Value>
		struct ReverseIterator {
		public:
			using ValueType     = std::remove_reference_t<Value>;
			using ReferenceType = ValueType &;
			using PointerType   = ValueType *;

		private:
			ValueType *ptr_;

		public:
			ReverseIterator &operator++() {
				ptr_++;
				return *this;
			}

			ReverseIterator &operator--() {
				ptr_--;
				return *this;
			}

			ReverseIterator operator++(int) {
				Iterator res = *this;
				ptr_++;
				return res;
			}

			ReverseIterator operator--(int) {
				Iterator res = *this;
				ptr_--;
				return res;
			}

			ReverseIterator operator+(size_t idx) const {
				return {.ptr_ = ptr_ - idx};
			}

			ReverseIterator operator-(size_t idx) const {
				return {.ptr_ = ptr_ + idx};
			}

			void operator+=(size_t idx) {
				ptr_ -= idx;
			}

			void operator-=(size_t idx) {
				ptr_ += idx;
			}

			size_t operator- (const ReverseIterator &other) const {
				return ptr_ - other.ptr_;
			}

			auto operator<=>(const ReverseIterator &other) const {
				return other.ptr_ <=> ptr_;
			}

			bool operator==(const ReverseIterator &other) const {
				return ptr_ == other.ptr_;
			}
		};


		template<class Value, size_t Len>
		class Array {
		public:
            static_assert(Len > 0, "The length of array should be larger than 0.");

            using Self = Array<Value, Len>;

            using ValueType     = std::remove_reference_t<Value>;
            using ReferenceType = ValueType &;
            using PointerType   = ValueType *;

        public:
			using IteratorType             = Iterator<ValueType>;
			using ConstIteratorType        = Iterator<const ValueType>;
			using ReverseIteratorType      = ReverseIterator<ValueType>;
			using ReverseConstIteratorType = ReverseIterator<const ValueType>;

			static_assert(iterator::RandomIterConcept<IteratorType>);
			static_assert(iterator::RandomIterConcept<ConstIteratorType>);
			static_assert(iterator::RandomIterConcept<ReverseIteratorType>);
			static_assert(iterator::RandomIterConcept<ReverseConstIteratorType>);

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
