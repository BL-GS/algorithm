/*
 * @author: BL-GS 
 * @date:   2023/5/3
 */

#pragma once
#ifndef ALGORITHM_STRUCTURE_LIST_H
#define ALGORITHM_STRUCTURE_LIST_H

#include <cstddef>
#include <cassert>
#include <cstdint>
#include <initializer_list>
#include <type_traits>
#include <algorithm>

#include <allocator/allocator.h>
#include <iterator/bilateral_iterator.h>

namespace algorithm::structure {

    inline namespace list {

		template<class Value>
		struct ListNode {
		public:
			Value value_;
			ListNode *next_ptr_;
			ListNode *prev_ptr_;

		public:
			ListNode() : next_ptr_(nullptr), prev_ptr_(nullptr) {}
			ListNode(const Value &value) : value_(value), next_ptr_(nullptr), prev_ptr_(nullptr) {}
			ListNode(Value &&value) : value_(std::forward<Value>(value)), next_ptr_(nullptr), prev_ptr_(nullptr) {}
		};

		template<class Value>
		struct Iterator {
		public:
			using ValueType     = std::remove_reference_t<Value>;
			using ReferenceType = ValueType &;
			using PointerType   = ValueType *;

			using NodeType  = ListNode<ValueType>;

		public:
			NodeType *ptr_;

		public:
			Iterator(): ptr_(nullptr) {}

			Iterator(const Iterator &other): ptr_(other.ptr_) {}

			Iterator(NodeType *ptr): ptr_(ptr) {}

		public:
			Iterator &operator++() {
				ptr_ = ptr_->next_ptr_;
				return *this;
			}

			Iterator &operator--() {
				ptr_ = ptr_->prev_ptr_;
				return *this;
			}

			Iterator operator++(int) {
				Iterator res = *this;
				ptr_ = ptr_->next_ptr_;
				return res;
			}

			Iterator operator--(int) {
				Iterator res = *this;
				ptr_ = ptr_->prev_ptr_;
				return res;
			}

			bool operator==(const Iterator &other) const {
				return ptr_ == other.ptr_;
			}

			Value &operator*() {
				return ptr_->value_;
			}

			Value *operator->() {
				return &(ptr_->value_);
			}
		};

		template<class Value>
		struct ReverseIterator {
		public:
			using ValueType     = std::remove_reference_t<Value>;
			using ReferenceType = ValueType &;
			using PointerType   = ValueType *;

			using NodeType = ListNode<ValueType>;

		public:
			NodeType *ptr_;

		public:
			ReverseIterator() : ptr_(nullptr) {}

			ReverseIterator(const ReverseIterator &other) : ptr_(other.ptr_) {}

			ReverseIterator(NodeType *ptr) : ptr_(ptr) {}

		public:
			ReverseIterator &operator++() {
				ptr_ = ptr_->prev_ptr_;
				return *this;
			}

			ReverseIterator &operator--() {
				ptr_ = ptr_->next_ptr_;
				return *this;
			}

			ReverseIterator operator++(int) {
				Iterator res = *this;
				ptr_ = ptr_->prev_ptr_;
				return res;
			}

			ReverseIterator operator--(int) {
				Iterator res = *this;
				ptr_ = ptr_->next_ptr_;
				return res;
			}

			bool operator==(const ReverseIterator &other) const {
				return ptr_ == other.ptr_;
			}

			Value &operator*() {
				return ptr_->value_;
			}

			Value *operator->() {
				return &(ptr_->value_);
			}
		};


		template<class Value, class Allocator = allocator::ReserveAllocator<Value>>
		    requires allocator::AllocatorConcept<Allocator>
		class List {
		public: 
			using Self = List<Value, Allocator>;

			using ValueType     = std::remove_reference_t<Value>;
            using ReferenceType = ValueType &;
            using PointerType   = ValueType *;

		private:
			using NodeType = ListNode<ValueType>;

		public:
			using AllocatorType = Allocator::template Rebind<NodeType>::type;

        public:
			using IteratorType             = Iterator<ValueType>;
			using ConstIteratorType        = Iterator<const ValueType>;
			using ReverseIteratorType      = ReverseIterator<ValueType>;
			using ReverseConstIteratorType = ReverseIterator<const ValueType>;

			static_assert(iterator::BilateralIterConcept<IteratorType>);
			static_assert(iterator::BilateralIterConcept<ConstIteratorType>);
			static_assert(iterator::BilateralIterConcept<ReverseIteratorType>);
			static_assert(iterator::BilateralIterConcept<ReverseConstIteratorType>);

		private:
			NodeType head_node_;

			size_t size_;

			AllocatorType allocator_;

		public:
            List(): size_(0) {
				head_node_.next_ptr_ = head_node_.prev_ptr_ = &head_node_;
			}

			List(const List &other): size_(0), allocator_(other.allocator_) {
				for (NodeType *cur_node_ptr = other.head_node_.next_ptr_; cur_node_ptr != &(other.head_node_); cur_node_ptr = cur_node_ptr->next_ptr_) {
					push_back(cur_node_ptr->value_);
				}
			}

			List(List &&other): size_(other.size_), allocator_(std::move(other.allocator_)) {
				head_node_.next_ptr_ = other.head_node_.next_ptr_;
				head_node_.prev_ptr_ = other.head_node_.prev_ptr_;

				other.size_ = 0;
				other.head_node_.next_ptr_ = other.head_node_.prev_ptr_ = &head_node_;
			}

            ~List() {
				while (head_node_.next_ptr_ != &head_node_) {
					NodeType *temp_ptr = head_node_.next_ptr_;
					head_node_.next_ptr_ = temp_ptr->next_ptr_;
					allocator_.deconstruct(temp_ptr);
				}
			}

        public:
			void push_front(const ValueType &new_value) {
				NodeType *new_node_ptr = allocator_.construct(new_value);
				new_node_ptr->next_ptr_ = head_node_.next_ptr_;
				new_node_ptr->prev_ptr_ = &head_node_;
				new_node_ptr->next_ptr_->prev_ptr_ = new_node_ptr;
				head_node_.next_ptr_ = new_node_ptr;

				++size_;
			}

			void push_back(const ValueType &new_value) {
				NodeType *new_node_ptr = allocator_.construct(new_value);
				new_node_ptr->next_ptr_ = &head_node_;
				new_node_ptr->prev_ptr_ = head_node_.prev_ptr_;
				new_node_ptr->prev_ptr_->next_ptr_ = new_node_ptr;
				head_node_.prev_ptr_ = new_node_ptr;

				++size_;
			}

			template<class ...Args>
			void emplace_front(Args &&...args) {
				NodeType *new_node_ptr = allocator_.construct(std::forward<Args>(args)...);
				new_node_ptr->next_ptr_ = head_node_.next_ptr_;
				new_node_ptr->prev_ptr_ = &head_node_;
				new_node_ptr->next_ptr_->prev_ptr_ = new_node_ptr;
				head_node_.next_ptr_ = new_node_ptr;

				++size_;
			}

			template<class ...Args>
			void emplace_back(Args &&...args) {
				NodeType *new_node_ptr = allocator_.construct(std::forward<Args>(args)...);
				new_node_ptr->next_ptr_ = &head_node_;
				new_node_ptr->prev_ptr_ = head_node_.prev_ptr_;
				new_node_ptr->prev_ptr_->next_ptr_ = new_node_ptr;
				head_node_.prev_ptr_ = new_node_ptr;

				++size_;
			}

			void pop_front() {
				NodeType *new_front_ptr = head_node_.next_ptr_->next_ptr_;
				allocator_.deconstruct(head_node_.next_ptr_);
				head_node_.next_ptr_ = new_front_ptr;
				new_front_ptr->prev_ptr_ = &head_node_;

				--size_;
			}

			void pop_back() {
				NodeType *new_back_ptr = head_node_.prev_ptr_->prev_ptr_;
				allocator_.deconstruct(head_node_.prev_ptr_);
				head_node_.prev_ptr_ = new_back_ptr;
				new_back_ptr->next_ptr_ = &head_node_;

				--size_;
			}

		public:
			IteratorType insert(size_t pos, const ValueType &new_value) {
				assert(pos < size_);

				NodeType *cur_node_ptr = &head_node_;
				while (pos > 0) {
					cur_node_ptr = cur_node_ptr->next_ptr_;
					--pos;
				}

				NodeType *new_node_ptr = allocator_.construct(new_value);
				new_node_ptr->next_ptr_ = cur_node_ptr;
				new_node_ptr->prev_ptr_ = cur_node_ptr->prev_ptr_;
				new_node_ptr->prev_ptr_->next_ptr_ = new_node_ptr;
				cur_node_ptr->prev_ptr_ = new_node_ptr;

				++size_;

				return {new_node_ptr};
			}

			IteratorType insert(IteratorType iter, const ValueType &new_value) {
				NodeType *cur_node_ptr = iter.ptr_;

				NodeType *new_node_ptr = allocator_.construct(new_value);
				new_node_ptr->next_ptr_ = cur_node_ptr;
				new_node_ptr->prev_ptr_ = cur_node_ptr->prev_ptr_;
				new_node_ptr->prev_ptr_->next_ptr_ = new_node_ptr;
				cur_node_ptr->prev_ptr_ = new_node_ptr;

				++size_;

				return {new_node_ptr};
			}

			IteratorType erase(size_t pos) {
				assert(pos < size_);

				NodeType *cur_node_ptr = &head_node_;
				while (pos > 0) {
					cur_node_ptr = cur_node_ptr->next_ptr_;
					--pos;
				}

				NodeType *res_ptr = cur_node_ptr->next_ptr_;
				cur_node_ptr->prev_ptr_->next_ptr_ = cur_node_ptr->next_ptr_;
				cur_node_ptr->next_ptr_->prev_ptr_ = cur_node_ptr->prev_ptr_;
				allocator_.deconstruct(cur_node_ptr);

				--size_;

				return {res_ptr};
			}

			IteratorType erase(IteratorType iter) {
				NodeType *cur_node_ptr = iter.ptr_;

				NodeType *res_ptr = cur_node_ptr->next_ptr_;
				cur_node_ptr->prev_ptr_->next_ptr_ = cur_node_ptr->next_ptr_;
				cur_node_ptr->next_ptr_->prev_ptr_ = cur_node_ptr->prev_ptr_;
				allocator_.deconstruct(cur_node_ptr);

				--size_;

				return {res_ptr};
			}

		public:
			ValueType & get(size_t pos) {
				assert(pos < size_);

				NodeType *cur_node_ptr = &head_node_;
				while (pos > 0) {
					cur_node_ptr = cur_node_ptr->next_ptr_;
					--pos;
				}

				return cur_node_ptr->value_;
			}

			void clear() {
				while (head_node_.next_ptr_ != &head_node_) {
					NodeType *temp_ptr = head_node_.next_ptr_;
					head_node_.next_ptr_ = temp_ptr->next_ptr_;
					allocator_.deconstruct(temp_ptr);
				}
				size_ = 0;
			}

			size_t size() const {
                return size_;
            }

        public:
            IteratorType begin() const {
                return IteratorType{head_node_.next_ptr_};
            }

			ConstIteratorType cbegin() const {
				return ConstIteratorType{head_node_.next_ptr_};
			}

			ReverseIteratorType rbegin() const {
				return ReverseIteratorType{head_node_.prev_ptr_};
			}

			ReverseConstIteratorType rcbegin() const {
				return ReverseConstIteratorType{head_node_.prev_ptr_};
			}

			IteratorType end() const {
				return IteratorType{const_cast<NodeType *>(&head_node_)};
			}

			ConstIteratorType cend() const {
				return ConstIteratorType{const_cast<NodeType *>(&head_node_)};
			}

			ReverseIteratorType rend() const {
				return ReverseIteratorType{const_cast<NodeType *>(&head_node_)};
			}

			ReverseConstIteratorType rcend() const {
				return ReverseConstIteratorType{const_cast<NodeType *>(&head_node_)};
			}
		};
	}

}

#endif
