/*
 * @author: BL-GS 
 * @date:   2023/5/11
 */

#pragma once
#ifndef ALGORITHM_STRUCTURE_BTREE_H
#define ALGORITHM_STRUCTURE_BTREE_H

#include <cstddef>
#include <algorithm>
#include <utility>

#include <util/compare.h>
#include <allocator/allocator.h>

namespace algorithm::structure {

	inline namespace btree {

		template<class Value, size_t WIDTH>
		struct BTreeNode {
		public:
			using ValueType = Value;

		public:
			size_t size_;

			ValueType value_[WIDTH];

			BTreeNode *child_ptr_[WIDTH + 1];

		public:
			BTreeNode(): size_(0) {
				std::fill(child_ptr_, child_ptr_ + WIDTH + 1, nullptr);
			}

		public:
			size_t size() const {
				return size_;
			}
		};

		template<class Value, size_t WIDTH = 8, class Cmp = util::Less, class Allocator = allocator::BaseAllocator>
		    requires allocator::AllocatorConcept<Allocator>
		class BTree {
		public:
			using Self = BTree<Value, WIDTH, Allocator>;

			using ValueType     = std::remove_reference_t<Value>;
			using ReferenceType = ValueType &;
			using PointerType   = ValueType *;

			using NodeType      = BTreeNode<ValueType, WIDTH>;

			using AllocatorType = Allocator;


			static constexpr size_t MAX_NODE_CAPACITY = WIDTH;
			static constexpr size_t MIN_NODE_CAPACITY = (WIDTH + 1) / 2;

		private:
			NodeType root_node_;

			size_t size_;

			AllocatorType allocator_;

		public:
			BTree() = default;

		public:
			bool insert(const ValueType &new_value) {
				auto [found, node_ptr, pos] = inner_find(new_value);

				if (found) { return false; }

				if (node_ptr.size() == MAX_NODE_CAPACITY) {

				}
				else {
					for (size_t i = node_ptr.size(); i > pos; --i) {
						node_ptr->value_[i] = node_ptr->value_[i - 1];
					}
					node_ptr->value_[pos] = new_value;
					++node_ptr->size_;
				}
			}

		private:
			std::tuple<bool, NodeType *, size_t> inner_find(NodeType *cur_node_ptr, const ValueType &value) const {
				for (size_t i = 0; i < cur_node_ptr->size(); ++i) {
					if (value == cur_node_ptr->value_[i]) {
						return { true, cur_node_ptr, i };
					}
					else if (!Cmp(value, cur_node_ptr->value_[i])) {
						NodeType *next_ptr = cur_node_ptr->child_ptr_[i];
						if (next_ptr == nullptr) {
							return { false, cur_node_ptr, i };
						}

						return inner_find(cur_node_ptr->child_ptr_[i], value);
					}
				}

				NodeType *next_ptr = cur_node_ptr->child_ptr_[cur_node_ptr->size()];
				if (next_ptr == nullptr) {
					return { false, cur_node_ptr, i };
				}

				return inner_find(cur_node_ptr->child_ptr_[cur_node_ptr->size()], value);
			}

			std::pair<NodeType *, size_t> inner_find(const ValueType &value) const {
				return inner_find(&root_node_, value);
			}

		};

	}

}

#endif//ALGORITHM_STRUCTURE_BTREE_H
