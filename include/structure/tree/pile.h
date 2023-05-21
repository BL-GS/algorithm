/*
 * @author: BL-GS 
 * @date:   2023/5/11
 */

#pragma once
#ifndef ALGORITHM_STRUCTURE_PILE_H
#define ALGORITHM_STRUCTURE_PILE_H

#include <cstdint>

#include <util/func/compare.h>
#include <allocator/allocator.h>

namespace algorithm::structure {

	inline namespace pile {

		template<class Value, class Cmp = util::Less, class Allocator = allocator::BaseAllocator>
		class Pile;

		template<class Value, class Cmp>
		struct PileNode {
		public:
			using ValueType = Value;

		public:
			Cmp compare_func_;

			ValueType value_;

			PileNode *left_ptr_;
			PileNode *right_ptr_;

		public:
			PileNode(const ValueType &value): value_(value), left_ptr_(nullptr), right_ptr_(nullptr) {}

			PileNode(ValueType &&value): value_(std::forward<ValueType>(value)), left_ptr_(nullptr), right_ptr_(nullptr) {}

		public:
			bool compare(const ValueType &other_value) const {
				return compare_func_(value_, other_value);
			}

			bool compare(const PileNode &other) const {
				return compare_func_(value_, other.value_);
			}
		};

		template<class Value, class Cmp, class Allocator>
		class Pile {
		public:
			using Self = Pile<Value, Allocator>;

			using ValueType     = std::remove_reference_t<Value>;
			using ReferenceType = ValueType &;
			using PointerType   = ValueType *;

			using NodeType      = PileNode<ValueType, Cmp>;

			using AllocatorType = Allocator;

		private:
			NodeType *root_ptr_;

			AllocatorType allocator_;

		public:
			Pile(): root_ptr_(nullptr) {}

			Pile(const std::initializer_list<ValueType> &value_list): root_ptr_(nullptr) {

			}

		public:
			void insert(const ValueType &new_value) {

			}

			ValueType &top() {
				return *root_ptr_;
			}

			void pop() {

			}

		private:
			void build_pile(const std::initializer_list<ValueType> &value_list) {
				NodeType *node_array = allocator_.allocate(sizeof(NodeType) * value_list.size());

				for (uint32_t idx = 0; idx < value_list.size(); ++idx) {
					new (node_array + idx) NodeType(value_list[idx]);
				}

				for (uint32_t idx = value_list.size() / 2; idx != 0; --idx) {
					uint32_t left_child_idx    = idx * 2 + 1;
					uint32_t right_child_idx   = left_child_idx + 1;

					node_array[idx].left_ptr_  = node_array + left_child_idx;
					if (right_child_idx < value_list.size()) [[unlikely]] {
						node_array[idx].right_ptr_ = node_array + right_child_idx;
					}
				}
			}

			bool adjust_pile(const NodeType *&to_root_ptr, const NodeType &root_node) {
				if (root_node.left_ptr_) {
					if (root_node.right_ptr_) {

					}
					else {
						if (root_node.compare(*root_node.left_ptr_)) {
							to_root_ptr = root_node.left_ptr_;
						}
						return false;
					}
				}
				return false;
			}
		};

	}

}

#endif//ALGORITHM_STRUCTURE_PILE_H
