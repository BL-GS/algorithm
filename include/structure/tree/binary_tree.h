/*
 * @author: BL-GS 
 * @date:   2023/5/18
 */

#pragma once
#ifndef ALGORITHM_STRUCTURE_BINARY_TREE_H
#define ALGORITHM_STRUCTURE_BINARY_TREE_H

#include <cstddef>
#include <cstdint>
#include <stack>
#include <queue>
#include <type_traits>

#include <iterator/bilateral_iterator.h>

namespace algorithm::structure {

	inline namespace binary_tree {

		template<class Value>
		struct TreeNode {
		public:
			using ValueType = Value;

		public:
			ValueType value_;
			TreeNode *left_child_;
			TreeNode *right_child_;

		public:
			TreeNode() = default;

			TreeNode(const ValueType &value): value_(value), left_child_(nullptr), right_child_(nullptr) {}

			TreeNode(ValueType &&value): value_(std::forward<ValueType>(value)), left_child_(nullptr), right_child_(nullptr) {}
		};

		template<class Value>
		struct Iterator {
		public:
			using ValueType     = std::remove_reference_t<Value>;
			using ReferenceType = ValueType &;
			using PointerType   = ValueType *;

			using NodeType      = TreeNode<ValueType>;

		public:
			NodeType *ptr_;

		public:
			Iterator(): ptr_(nullptr) {}

			Iterator(const Iterator &other): ptr_(other.ptr_) {}

			Iterator(NodeType *ptr): ptr_(ptr) {}

		public:
			Iterator &operator++() {
				ptr_ = ptr_->right_child_;
				return *this;
			}

			Iterator &operator--() {
				ptr_ = ptr_->left_child_;
				return *this;
			}

			Iterator operator++(int) {
				Iterator res = *this;
				ptr_ = ptr_->right_child_;
				return res;
			}

			Iterator operator--(int) {
				Iterator res = *this;
				ptr_ = ptr_->left_child_;
				return res;
			}

			bool operator==(const Iterator &other) const {
				return ptr_ == other.ptr_;
			}

			ValueType &operator*() {
				return ptr_->value_;
			}

			ValueType *operator->() {
				return &(ptr_->value_);
			}
		};

		template<class Value>
		struct ReverseIterator {
		public:
			using ValueType     = std::remove_reference_t<Value>;
			using ReferenceType = ValueType &;
			using PointerType   = ValueType *;

			using NodeType = TreeNode<ValueType>;

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

		template<class Value, class Allocator>
		class SimpleBinaryTree {
		public:
			using Self = SimpleBinaryTree<Value, Allocator>;

			using ValueType     = std::remove_reference_t<Value>;
			using ReferenceType = ValueType &;
			using PointerType   = ValueType *;

		private:
			using NodeType      = TreeNode<ValueType>;

		public:
			using AllocatorType = Allocator::template Rebind<NodeType>::type;

		private:
			using IteratorType             = Iterator<ValueType>;
			using ConstIteratorType        = Iterator<const ValueType>;
			using ReverseIteratorType      = ReverseIterator<ValueType>;
			using ReverseConstIteratorType = ReverseIterator<const ValueType>;

			static_assert(iterator::BilateralIterConcept<IteratorType>);
			static_assert(iterator::BilateralIterConcept<ConstIteratorType>);
			static_assert(iterator::BilateralIterConcept<ReverseIteratorType>);
			static_assert(iterator::BilateralIterConcept<ReverseConstIteratorType>);

			static constexpr NodeType *VACANCY_TAIL_NODE_PTR_ = nullptr;

		private:
			union {
				uint8_t vacancy[1];
				NodeType root_node_ptr_;
			};

			size_t size_;

			AllocatorType allocator_;

		public:
			SimpleBinaryTree(): size_(0) {
				root_node_ptr_.left_child_ = root_node_ptr_.right_child_ = VACANCY_TAIL_NODE_PTR_;
			}

			~SimpleBinaryTree() = default;

		public:
			IteratorType insert_left(IteratorType iter, const ValueType &new_value) {
				NodeType *parent_node_ptr    = iter.ptr_;
				NodeType *new_node_ptr       = allocator_.construct(new_value);

				new_node_ptr->right_child_   = VACANCY_TAIL_NODE_PTR_;
				new_node_ptr->left_child_    = parent_node_ptr->left_child_;
				parent_node_ptr->left_child_ = new_node_ptr;

				++size_;
				return {new_node_ptr};
			}

			IteratorType insert_right(IteratorType iter, const ValueType &new_value) {
				NodeType *parent_node_ptr    = iter.ptr_;
				NodeType *new_node_ptr       = allocator_.construct(new_value);

				new_node_ptr->left_child_     = VACANCY_TAIL_NODE_PTR_;
				new_node_ptr->right_child_    = parent_node_ptr->right_child_;
				parent_node_ptr->right_child_ = new_node_ptr;

				++size_;
				return {new_node_ptr};
			}

			IteratorType erase_left(IteratorType iter) {
				NodeType *parent_node_ptr    = iter.ptr_;
				NodeType *del_node_ptr       = parent_node_ptr->left_child_;
				NodeType *new_left_child_ptr = del_node_ptr->left_child_;

				if (del_node_ptr->right_child_ == VACANCY_TAIL_NODE_PTR_) {
					allocator_.deconstruct(del_node_ptr);
					parent_node_ptr->left_child_ = new_left_child_ptr;

					--size_;
					return {new_left_child_ptr};
				}
				return iter;
			}

			IteratorType erase_right(IteratorType iter) {
				NodeType *parent_node_ptr     = iter.ptr_;
				NodeType *del_node_ptr        = parent_node_ptr->right_child_;
				NodeType *new_right_child_ptr = del_node_ptr->right_child_;

				if (del_node_ptr->right_child_ == VACANCY_TAIL_NODE_PTR_) {
					allocator_.deconstruct(del_node_ptr);
					parent_node_ptr->right_child_ = new_right_child_ptr;

					--size_;
					return {new_right_child_ptr};
				}
				return iter;
			}

		public:
			void swap_children(IteratorType iter) {
				NodeType *node_ptr = iter.ptr_;
				std::swap(node_ptr->left_child_, node_ptr->right_child_);
			}

			void swap_node(IteratorType iter_a, IteratorType iter_b)
			    requires std::is_swappable_v<ValueType> {
				NodeType *node_a_ptr_ = iter_a.ptr_;
				NodeType *node_b_ptr_ = iter_b.ptr_;
				std::swap(node_a_ptr_->value_, node_b_ptr_->value_);
			}

			void swap_left_children(IteratorType iter_a, IteratorType iter_b) {
				NodeType *node_a_ptr_ = iter_a.ptr_;
				NodeType *node_b_ptr_ = iter_b.ptr_;
				std::swap(node_a_ptr_->left_child_, node_b_ptr_->left_child_);
			}

			void swap_right_children(IteratorType iter_a, IteratorType iter_b) {
				NodeType *node_a_ptr_ = iter_a.ptr_;
				NodeType *node_b_ptr_ = iter_b.ptr_;
				std::swap(node_a_ptr_->right_child_, node_b_ptr_->right_child_);
			}

		public:
			template<class Func>
			void dfs_traverse(IteratorType start_iter, Func &func) {
				if (start_iter == end()) { return; }

				std::stack<IteratorType> iter_stack;
				iter_stack.emplace(start_iter);

				while (!iter_stack.empty()) {
					IteratorType cur_iter = iter_stack.top();
					iter_stack.pop();

					IteratorType left_child_iter  = cur_iter; --left_child_iter;
					IteratorType right_child_iter = cur_iter; ++right_child_iter;

					NodeType *cur_node            = cur_iter.ptr_;
					NodeType *left_child_ptr      = left_child_iter.ptr_;
					NodeType *right_child_ptr     = right_child_iter.ptr_;

					if (right_child_ptr != VACANCY_TAIL_NODE_PTR_) {
						iter_stack.emplace(right_child_iter);
					}
					if (left_child_iter != VACANCY_TAIL_NODE_PTR_) {
						iter_stack.emplace(left_child_iter);
					}

					func(cur_node->value_);
				}
			}

			template<class Func>
			void dfs_traverse(Func &func) {
				dfs_traverse(begin(), func);
			}

			template<class Func>
			void bfs_traverse(IteratorType start_iter, Func &func) {
				if (start_iter == end()) { return; }

				std::queue<IteratorType> iter_stack;
				iter_stack.emplace(start_iter);

				while (!iter_stack.empty()) {
					IteratorType cur_iter = iter_stack.front();
					iter_stack.pop();

					IteratorType left_child_iter  = cur_iter; --left_child_iter;
					IteratorType right_child_iter = cur_iter; ++right_child_iter;

					NodeType *cur_node            = cur_iter.ptr_;
					NodeType *left_child_ptr      = left_child_iter.ptr_;
					NodeType *right_child_ptr     = right_child_iter.ptr_;

					if (left_child_iter != VACANCY_TAIL_NODE_PTR_) {
						iter_stack.emplace(left_child_iter);
					}
					if (right_child_ptr != VACANCY_TAIL_NODE_PTR_) {
						iter_stack.emplace(right_child_iter);
					}

					func(cur_node->value_);
				}
			}

			template<class Func>
			void bfs_traverse(Func &func) {
				bfs_traverse(begin(), func);
			}

		public:

			size_t size() const {
				return size_;
			}

		public:
			IteratorType begin() const {
				return {root_node_ptr_};
			}

			IteratorType end() const {
				return {VACANCY_TAIL_NODE_PTR_};
			}

			ConstIteratorType cbegin() const {
				return {root_node_ptr_};
			}

			ConstIteratorType cend() const {
				return {VACANCY_TAIL_NODE_PTR_};
			}

			ReverseIteratorType rbegin() const {
				return {root_node_ptr_};
			}

			ReverseIteratorType rend() const {
				return {VACANCY_TAIL_NODE_PTR_};
			}

			ReverseConstIteratorType rcbegin() const {
				return {root_node_ptr_};
			}

			ReverseConstIteratorType rcend() const {
				return {VACANCY_TAIL_NODE_PTR_};
			}
		};

	}

}

#endif//ALGORITHM_STRUCTURE_BINARY_TREE_H
