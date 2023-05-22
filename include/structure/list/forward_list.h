/*
 * @author: BL-GS 
 * @date:   2023/5/3
 */

#pragma once
#ifndef ALGORITHM_STRUCTURE_FORWARD_LIST_H
#define ALGORITHM_STRUCTURE_FORWARD_LIST_H

#include <cstddef>
#include <cassert>
#include <cstdint>
#include <initializer_list>
#include <type_traits>
#include <algorithm>

#include <allocator/allocator.h>
#include <iterator/forwarding_iterator.h>

namespace algorithm::structure {

    inline namespace forward_list {

		namespace detail {
			template<class Value>
			struct ListNode {
			public:
				Value value_;
				ListNode *next_ptr_;

			public:
				ListNode() : next_ptr_(nullptr) {}
				ListNode(const Value &value) : value_(value), next_ptr_(nullptr) {}
				ListNode(Value &&value) : value_(std::forward<Value>(value)), next_ptr_(nullptr) {}
			};

			template<class Value>
			struct Iterator: iterator::ForwardingIteratorCRTP<Iterator<Value>, Value> {
			public:
				using ValueType     = std::remove_reference_t<Value>;
				using ReferenceType = ValueType &;
				using PointerType   = ValueType *;

				using NodeType      = ListNode<ValueType>;

			public:
				NodeType *ptr_;

			public:
				Iterator() = default;

				Iterator(NodeType *ptr): ptr_(ptr) {}

			public:
				ReferenceType dereference() const { return ptr_->value_; }

			public:
				void increment() { ptr_ = ptr_->next_ptr_; }

			public:
				bool equal(const Iterator &other) const { return ptr_ == other.ptr_; }
			};
		}

		template<class Value, class Allocator = allocator::ReserveAllocator<Value>>
		    requires allocator::AllocatorConcept<Allocator>
		class ForwardList {
		public: 
			using Self = ForwardList<Value, Allocator>;

			using ValueType     = std::remove_reference_t<Value>;
            using ReferenceType = ValueType &;
            using PointerType   = ValueType *;

		public:
			using NodeType = detail::ListNode<ValueType>;

		public:
			using AllocatorType = Allocator::template Rebind<NodeType>::type;

        public:
			using IteratorType             = detail::Iterator<ValueType>;
			using ConstIteratorType        = detail::Iterator<const ValueType>;

		private:
			NodeType head_node_;

			size_t size_;

			AllocatorType allocator_;

		public:
            ForwardList(): size_(0) {
				head_node_.next_ptr_ = &head_node_;
			}

			ForwardList(const ForwardList &other): size_(0), allocator_(other.allocator_) {
				NodeType insert_node_ptr = &head_node_;
				for (NodeType *cur_node_ptr = other.head_node_.next_ptr_; cur_node_ptr != &(other.head_node_); cur_node_ptr = cur_node_ptr->next_ptr_) {
					NodeType *new_node_ptr = allocator_.construct(cur_node_ptr->value_);
					new_node_ptr->next_ptr_ = insert_node_ptr->next_ptr_;
					insert_node_ptr->next_ptr_ = new_node_ptr;
					insert_node_ptr = new_node_ptr;
				}
			}

			ForwardList(ForwardList &&other): size_(other.size_), allocator_(std::move(other.allocator_)) {
				head_node_.next_ptr_ = other.head_node_.next_ptr_;

				other.size_ = 0;
				other.head_node_.next_ptr_ = &head_node_;
				other.tail_node_ptr_ = &head_node_;
			}

            ~ForwardList() {
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
				head_node_.next_ptr_ = new_node_ptr;

				++size_;
			}

			void push_back(const ValueType &new_value) {
				NodeType *new_node_ptr = allocator_.construct(new_value);

				NodeType *cur_node_ptr;
				for (cur_node_ptr = head_node_.next_ptr_; cur_node_ptr->next_ptr_ != &head_node_;) {
					cur_node_ptr = cur_node_ptr->next_ptr_;
				}

				new_node_ptr->next_ptr_ = cur_node_ptr->next_ptr_;
				cur_node_ptr->next_ptr_ = new_node_ptr;

				++size_;
			}

			template<class ...Args>
			void emplace_front(Args &&...args) {
				NodeType *new_node_ptr = allocator_.construct(std::forward<Args>(args)...);
				new_node_ptr->next_ptr_ = head_node_.next_ptr_;
				head_node_.next_ptr_ = new_node_ptr;

				++size_;
			}

			template<class ...Args>
			void emplace_back(Args &&...args) {
				NodeType *new_node_ptr = allocator_.construct(std::forward<Args>(args)...);
				NodeType *cur_node_ptr;
				for (cur_node_ptr = head_node_.next_ptr_; cur_node_ptr->next_ptr_ != &head_node_;) {
					cur_node_ptr = cur_node_ptr->next_ptr_;
				}

				new_node_ptr->next_ptr_ = cur_node_ptr->next_ptr_;
				cur_node_ptr->next_ptr_ = new_node_ptr;

				++size_;
			}

			void pop_front() {
				NodeType *new_front_ptr = head_node_.next_ptr_->next_ptr_;
				allocator_.deconstruct(head_node_.next_ptr_);
				head_node_.next_ptr_ = new_front_ptr;

				--size_;
			}

			void pop_back() {
				NodeType *pre_node_ptr = &head_node_;
				NodeType *cur_node_ptr;
				for (cur_node_ptr = head_node_.next_ptr_; cur_node_ptr->next_ptr_ != &head_node_;) {
					pre_node_ptr = cur_node_ptr;
					cur_node_ptr = cur_node_ptr->next_ptr_;
				}
				pre_node_ptr->next_ptr_ = cur_node_ptr->next_ptr_;
				allocator_.deconstruct(cur_node_ptr);

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
				new_node_ptr->next_ptr_ = cur_node_ptr->next_ptr_;
				cur_node_ptr->next_ptr_ = new_node_ptr;

				++size_;

				return {new_node_ptr};
			}

			IteratorType insert_next(IteratorType iter, const ValueType &new_value) {
				NodeType *cur_node_ptr = iter.ptr_;

				NodeType *new_node_ptr  = allocator_.construct(new_value);
				new_node_ptr->next_ptr_ = cur_node_ptr->next_ptr_;
				cur_node_ptr->next_ptr_ = new_node_ptr;

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

				NodeType *del_node_ptr = cur_node_ptr->next_ptr_;
				cur_node_ptr->next_ptr_ = cur_node_ptr->next_ptr_->next_ptr_;
				allocator_.deconstruct(del_node_ptr);

				--size_;

				return {cur_node_ptr->next_ptr_};
			}

			IteratorType erase_next(IteratorType pre_iter) {
				NodeType *cur_node_ptr = pre_iter.ptr_;

				NodeType *del_node_ptr = cur_node_ptr->next_ptr_;
				cur_node_ptr->next_ptr_ = cur_node_ptr->next_ptr_->next_ptr_;
				allocator_.deconstruct(del_node_ptr);

				--size_;

				return {cur_node_ptr};
			}

		public:
			ValueType & get(size_t pos) {
				assert(pos < size_);

				NodeType *cur_node_ptr = head_node_.next_ptr_;
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

			IteratorType end() const {
				return IteratorType{const_cast<NodeType *>(&head_node_)};
			}

			ConstIteratorType cend() const {
				return ConstIteratorType{const_cast<NodeType *>(&head_node_)};
			}
		};
	}

}

#endif
