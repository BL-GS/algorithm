/*
 * @author: BL-GS 
 * @date:   2023/5/12
 */

#pragma once
#ifndef ALGORITHM_STRUCTURE_SMALL_VECTOR_H
#define ALGORITHM_STRUCTURE_SMALL_VECTOR_H

#include <cstdint>
#include <cstring>

#include <allocator/allocator.h>
#include <iterator/random_iterator.h>

namespace algorithm::structure {

	inline namespace small_vector {

		template<class Value>
		struct Iterator {
		public:
			using ValueType     = std::remove_reference_t<Value>;
			using ReferenceType = ValueType &;
			using PointerType   = ValueType *;

		public:
			ValueType *ptr_;

		public:
			Iterator(): ptr_(nullptr) {}

			Iterator(const Iterator &other): ptr_(other.ptr_) {}

			Iterator(ValueType *ptr): ptr_(ptr) {}

		public:
			Iterator &operator++() {
				++ptr_;
				return *this;
			}

			Iterator &operator--() {
				--ptr_;
				return *this;
			}

			Iterator operator++(int) {
				return Iterator(ptr_++);
			}

			Iterator operator--(int) {
				return Iterator(ptr_--);
			}

			Iterator &operator+=(size_t step) {
				ptr_ += step;
				return *this;
			}

			Iterator &operator-=(size_t step) {
				ptr_ -= step;
				return *this;
			}

			Iterator operator+ (size_t step) const {
				return Iterator(ptr_ + step);
			}

			Iterator operator- (size_t step) const {
				return Iterator(ptr_ - step);
			}

			size_t operator- (const Iterator &other) const {
				return other.ptr_ - ptr_;
			}

			bool operator==(const Iterator &other) const {
				return ptr_ == other.ptr_;
			}

			auto operator<=> (const Iterator &other) const {
		        return ptr_ <=> other.ptr_;
			}

			ValueType &operator*() {
				return *ptr_;
			}

			ValueType *operator->() {
				return ptr_;
			}
		};

		template<class Value>
		struct ReverseIterator {
		public:
			using ValueType     = std::remove_reference_t<Value>;
			using ReferenceType = ValueType &;
			using PointerType   = ValueType *;

		public:
			ValueType *ptr_;

		public:
			ReverseIterator() : ptr_(nullptr) {}

			ReverseIterator(const ReverseIterator &other) : ptr_(other.ptr_) {}

			ReverseIterator(ValueType *ptr) : ptr_(ptr) {}

		public:
			ReverseIterator &operator++() {
				--ptr_;
				return *this;
			}

			ReverseIterator &operator--() {
				++ptr_;
				return *this;
			}

			ReverseIterator operator++(int) const {
				return ReverseIterator(ptr_--);
			}

			ReverseIterator operator--(int) const {
				return ReverseIterator(ptr_++);
			}

			ReverseIterator &operator+=(size_t step) {
				ptr_ -= step;
				return *this;
			}

			ReverseIterator &operator-=(size_t step) {
				ptr_ += step;
				return *this;
			}

			ReverseIterator operator+ (size_t step) const {
				return Iterator(ptr_ - step);
			}

			ReverseIterator operator- (size_t step) const {
				return Iterator(ptr_ + step);
			}

			size_t operator- (const ReverseIterator &other) const {
				return ptr_ - other.ptr_;
			}

			bool operator==(const ReverseIterator &other) const {
				return ptr_ == other.ptr_;
			}

			auto operator<=> (const ReverseIterator &other) const {
				return other.ptr_ <=> ptr_;
			}

			ValueType &operator*() {
				return *ptr_;
			}

			ValueType *operator->() {
				return ptr_;
			}
		};


		template<class Value, size_t FIXED_SIZE = 2, class Allocator = allocator::ReserveAllocator<Value>>
		class SmallVector {
		public:
			using Self = SmallVector<Value, FIXED_SIZE, Allocator>;

			using ValueType     = std::remove_reference_t<Value>;
			using ReferenceType = ValueType &;
			using PointerType   = ValueType *;

			using AllocatorType = Allocator;

			static constexpr size_t FIXED_CAPACITY_SIZE = sizeof(ValueType) * FIXED_SIZE;

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
			size_t size_;

			size_t capacity_;

			ValueType *content_ptr_;

			uint8_t fixed_capacity_[FIXED_CAPACITY_SIZE];

			AllocatorType allocator_;

		public:
			SmallVector(): size_(0), capacity_(FIXED_SIZE), content_ptr_(reinterpret_cast<ValueType *>(fixed_capacity_)) {}

			~SmallVector() noexcept {
				clear();
			}

		public:
			ValueType &operator[] (size_t idx) {
				return content_ptr_[idx];
			}

		public:
			IteratorType push_back(const ValueType &value) {
				expand_with_insert(size_++, value);
				return { content_ptr_ + size_ - 1 };
			}

			template<class ...Args>
			IteratorType emplace_back(Args &&...args) {
				expand_with_insert(size_++, std::forward<Args>(args)...);
				return { content_ptr_ + size_ - 1 };
			}

			IteratorType insert(size_t pos, const ValueType &value) {
				++size_;

				ValueType *origin_content_ptr_ = content_ptr_;
				expand_with_insert(pos, value);

				return { content_ptr_ + pos };
			}

			IteratorType insert(IteratorType iter, const ValueType &value) {
				size_t pos = iter.ptr_ - content_ptr_;
				return insert(pos, value);
			}

			IteratorType erase(size_t pos) {
				--size_;
				shrink_with_erase(pos);

				return { content_ptr_ + pos };
			}

			IteratorType erase(IteratorType iter) {
				ValueType *target_ptr = iter.ptr_;
				size_t pos = target_ptr - content_ptr_;

				--size_;
				shrink_with_erase(pos);

				return { content_ptr_ + pos };
			}

		public:
			ValueType &front() {
				return content_ptr_[0];
			}

			ValueType &back() {
				return content_ptr_[size_ - 1];
			}

			void clear() {
				size_ = 0;

				ValueType *fixed_ptr = reinterpret_cast<ValueType *>(fixed_capacity_);
				if (content_ptr_ != fixed_ptr) {
					allocator_.deallocate(content_ptr_, capacity_);
					content_ptr_ = fixed_ptr;
					capacity_ = FIXED_SIZE;
				}
			}

			size_t size() const {
				return size_;
			}

			size_t capacity() const {
				return capacity_;
			}

		private:
			template<class... Args>
			void expand_with_insert(size_t pos, Args &&...args) {
				if (size_ <= capacity_) {
					for (size_t i = pos; i < size_ - 1; ++i) {
						new (content_ptr_ + i + 1) ValueType(std::move(content_ptr_[i]));
						content_ptr_[i].~ValueType();
					}
					new (content_ptr_ + pos) ValueType(std::forward<Args>(args)...);

					return;
				}

				size_t new_capacity = capacity_ << 1;

				if (content_ptr_ == reinterpret_cast<ValueType *>(fixed_capacity_)) {
					content_ptr_ = static_cast<ValueType *>(
					        allocator_.allocate(new_capacity)
					);

					for (size_t i = 0; i < pos; ++i) {
						new (content_ptr_ + i) ValueType(std::move(fixed_capacity_[i]));
						fixed_capacity_[i].~ValueType();
					}
					new (content_ptr_ + pos) ValueType(std::forward<Args>(args)...);
					for (size_t i = pos; i < capacity_; ++i) {
						new (content_ptr_ + i + 1) ValueType(std::move(fixed_capacity_[i]));
						fixed_capacity_[i].~ValueType();
					}
				}
				else {
					ValueType *origin_content_ptr = content_ptr_;
					content_ptr_ = static_cast<ValueType *>(
					        allocator_.allocate(new_capacity)
					);

					for (size_t i = 0; i < capacity_; ++i) {
						new (content_ptr_ + i) ValueType(std::move(origin_content_ptr[i]));
						origin_content_ptr[i].~ValueType();
					}

					allocator_.deallocate(content_ptr_, capacity_);
				}
				capacity_ = new_capacity;
			}

			void shrink_with_erase(size_t pos) {
				size_t new_capacity_ = capacity_ / 2;

				if (size_ > new_capacity_ / 2) {
					for (size_t i = pos; i < capacity_ - 1; ++i) {
						content_ptr_[i].~ValueType();
						new (content_ptr_ + i) ValueType(std::move(content_ptr_[i + 1]));
					}
					content_ptr_[capacity_].~ValueType();
				}
				else if (size_ <= FIXED_SIZE / 2) {
					ValueType *fixed_ptr = reinterpret_cast<ValueType *>(fixed_capacity_);
					if (content_ptr_ != fixed_ptr) {
						for (size_t i = 0; i < pos; ++i) {
							new (fixed_ptr + i) ValueType(std::move(content_ptr_[i]));
							content_ptr_[i].~ValueType();
						}
						for (size_t i = pos; i < capacity_ - 1; ++i) {
							new (fixed_ptr + i) ValueType(std::move(content_ptr_[i + 1]));
							content_ptr_[i].~ValueType();
						}

						allocator_.deallocate(content_ptr_, capacity_);
						content_ptr_ = fixed_ptr;

						capacity_ = FIXED_SIZE;
					}
				}
				else {
					ValueType *new_content_ptr = allocator_.allocate(content_ptr_, capacity_, new_capacity_);

					for (size_t i = 0; i < pos; ++i) {
						new (new_content_ptr + i) ValueType(std::move(content_ptr_[i]));
						content_ptr_[i].~ValueType();
					}
					for (size_t i = pos; i < capacity_ - 1; ++i) {
						new (new_content_ptr + i) ValueType(std::move(content_ptr_[i + 1]));
						content_ptr_[i].~ValueType();
					}

					allocator_.deallocate(content_ptr_, capacity_);
					content_ptr_ = new_content_ptr;
					capacity_ >>= 1;
				}
			}

		public:
			IteratorType begin() const {
				return IteratorType{ content_ptr_ };
			}

			ConstIteratorType cbegin() const {
				return ConstIteratorType{ content_ptr_ };
			}

			ReverseIteratorType rbegin() const {
				return ReverseIteratorType{ content_ptr_ + size_ - 1 };
			}

			ReverseConstIteratorType rcbegin() const {
				return ReverseConstIteratorType{ content_ptr_ + size_ - 1 };
			}

			IteratorType end() const {
				return IteratorType{ content_ptr_ + size_ };
			}

			ConstIteratorType cend() const {
				return ConstIteratorType{ content_ptr_ + size_ };
			}

			ReverseIteratorType rend() const {
				return ReverseIteratorType{ content_ptr_ - 1 };
			}

			ReverseConstIteratorType rcend() const {
				return ReverseConstIteratorType{ content_ptr_ - 1 };
			}
		};

		template<class Value, size_t FIXED_SIZE, class Allocator>
			requires std::is_trivial_v<Value>
		class SmallVector<Value, FIXED_SIZE, Allocator> {
		public:
			using Self = SmallVector<Value, FIXED_SIZE, Allocator>;

			using ValueType     = std::remove_reference_t<Value>;
			using ReferenceType = ValueType &;
			using PointerType   = ValueType *;

			using AllocatorType = Allocator;

			static constexpr size_t FIXED_CAPACITY_SIZE = sizeof(ValueType) * FIXED_SIZE;

		public:
			using IteratorType             = Iterator<ValueType>;
			using ConstIteratorType        = Iterator<const ValueType>;
			using ReverseIteratorType      = ReverseIterator<ValueType>;
			using ReverseConstIteratorType = ReverseIterator<const ValueType>;

		private:
			size_t size_;

			size_t capacity_;

			ValueType *content_ptr_;

			uint8_t fixed_capacity_[FIXED_CAPACITY_SIZE];

			AllocatorType allocator_;

		public:
			constexpr SmallVector(): size_(0), capacity_(FIXED_SIZE),
			                          content_ptr_(reinterpret_cast<ValueType *>(fixed_capacity_)) {}

			~SmallVector() noexcept {
				clear();
			}

		public:
			ValueType &operator[] (size_t idx) {
				return content_ptr_[idx];
			}

		public:
			IteratorType push_back(const ValueType &value) {
				size_t last_idx = size_++;
				expand();
				content_ptr_[last_idx] = value;

				return { content_ptr_ + last_idx };
			}

			template<class ...Args>
			IteratorType emplace_back(Args &&...args) {
				size_t last_idx = size_++;
				expand();
				util::construct<ValueType>(content_ptr_ + last_idx, std::forward<Args>(args)...);

				return { content_ptr_ + last_idx };
			}

			IteratorType insert(size_t pos, const ValueType &value) {
				assert(pos < size_);

				++size_;

				ValueType *origin_content_ptr_ = content_ptr_;
				expand();

				ValueType *target_ptr = content_ptr_ + pos;
				if (origin_content_ptr_ != content_ptr_) {
					std::memmove(target_ptr + 1, target_ptr, (size_ - 1 - pos) * sizeof(ValueType));
				}
				else {
					std::memcpy(target_ptr + 1, target_ptr, (size_ - 1 - pos) * sizeof(ValueType));
				}
				*target_ptr = value;

				return { target_ptr };
			}

			IteratorType insert(IteratorType iter, const ValueType &value) {
				size_t pos = iter.ptr_ - content_ptr_;
				assert(pos < size_);

				++size_;

				ValueType *origin_content_ptr_ = content_ptr_;
				expand();

				ValueType *target_ptr = content_ptr_ + pos;
				if (origin_content_ptr_ != content_ptr_) {
					std::memmove(target_ptr + 1, target_ptr, (size_ - 1 - pos) * sizeof(ValueType));
				}
				else {
					std::memcpy(target_ptr + 1, target_ptr, (size_ - 1 - pos) * sizeof(ValueType));
				}
				*target_ptr = value;

				return { target_ptr };
			}

			IteratorType erase(size_t pos) {
				assert(pos < size_);

				ValueType *target_ptr = content_ptr_ + pos;
				std::memmove(target_ptr, target_ptr + 1, (size_ - 1 - pos) * sizeof(ValueType));

				--size_;
				shrink();

				return { content_ptr_ + pos };
			}

			IteratorType erase(IteratorType iter) {
				ValueType *target_ptr = iter.ptr_;
				size_t pos = target_ptr - content_ptr_;
				assert(pos < size_);

				std::memmove(target_ptr, target_ptr + 1, (size_ - 1 - pos) * sizeof(ValueType));

				--size_;
				shrink();

				return { content_ptr_ + pos };
			}

		public:
			ValueType &front() {
				return content_ptr_[0];
			}

			ValueType &back() {
				return content_ptr_[size_ - 1];
			}

			void clear() {
				size_ = 0;

				ValueType *fixed_ptr = reinterpret_cast<ValueType *>(fixed_capacity_);
				if (content_ptr_ != fixed_ptr) {
					allocator_.deallocate(content_ptr_, capacity_);
					content_ptr_ = fixed_ptr;
					capacity_ = FIXED_SIZE;
				}
			}

			size_t size() const {
				return size_;
			}

			size_t capacity() const {
				return capacity_;
			}

		private:
			void expand() {
				if (size_ <= capacity_) { return ; }

				size_t new_capacity = capacity_ << 1;

				if (content_ptr_ == reinterpret_cast<ValueType *>(fixed_capacity_)) {
					content_ptr_ = static_cast<ValueType *>(
					        allocator_.allocate(new_capacity)
					);
					std::memcpy(content_ptr_, fixed_capacity_, capacity_ * sizeof(ValueType));
				}
				else {
					content_ptr_ = static_cast<ValueType *>(
					        allocator_.reallocate(content_ptr_, capacity_, new_capacity)
					);
				}
				capacity_ = new_capacity;
			}


			void shrink() {

				size_t new_capacity_ = capacity_ / 2;

				if (size_ > new_capacity_ / 2) { return ; }

				if (size_ <= FIXED_SIZE / 2) {
					ValueType *fixed_ptr = reinterpret_cast<ValueType *>(fixed_capacity_);
					if (content_ptr_ != fixed_ptr) {
						std::memcpy(fixed_ptr, content_ptr_, size_ * sizeof(ValueType));

						allocator_.deallocate(content_ptr_, capacity_);
						content_ptr_ = fixed_ptr;

						capacity_ = FIXED_SIZE;
					}
				}
				else {
					content_ptr_ = static_cast<ValueType *>(allocator_.reallocate(content_ptr_, capacity_, new_capacity_));
					capacity_ = new_capacity_;
				}
			}

		public:
			IteratorType begin() const {
				return IteratorType{ content_ptr_ };
			}

			ConstIteratorType cbegin() const {
				return ConstIteratorType{ content_ptr_ };
			}

			ReverseIteratorType rbegin() const {
				return ReverseIteratorType{ content_ptr_ + size_ - 1 };
			}

			ReverseConstIteratorType rcbegin() const {
				return ReverseConstIteratorType{ content_ptr_ + size_ - 1 };
			}

			IteratorType end() const {
				return IteratorType{ content_ptr_ + size_ };
			}

			ConstIteratorType cend() const {
				return ConstIteratorType{ content_ptr_ + size_ };
			}

			ReverseIteratorType rend() const {
				return ReverseIteratorType{ content_ptr_ - 1 };
			}

			ReverseConstIteratorType rcend() const {
				return ReverseConstIteratorType{ content_ptr_ - 1 };
			}
		};


	}
}

#endif//ALGORITHM_STRUCTURE_SMALL_VECTOR_H
