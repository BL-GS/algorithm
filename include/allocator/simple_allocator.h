/*
 * @author: BL-GS 
 * @date:   2023/5/13
 */

#pragma once
#ifndef ALGORITHM_ALLOCATOR_SIMPLE_ALLOCATOR_H
#define ALGORITHM_ALLOCATOR_SIMPLE_ALLOCATOR_H

#include <cstddef>
#include <cstdlib>
#include <utility>
#include <concepts>

#include <util/func/type.h>
#include <allocator/base_allocator.h>

namespace algorithm::allocator {

	template<class T>
	class SimpleAllocator {
	public:
		using AllocateType = T;

		template<class U>
		struct Rebind {
			using type = SimpleAllocator<U>;
		};

	private:
		BaseAllocator base_allocator_;

	public:
		SimpleAllocator() = default;

		SimpleAllocator(const SimpleAllocator &other) = default;

		~SimpleAllocator() = default;

	public:
		template<class ...Args>
		AllocateType *construct(Args &&... args) {
			return util::construct<AllocateType>(allocate(sizeof(T)), std::forward<Args>(args)...);
		}

		void deconstruct(T *ptr) {
			deallocate(util::deconstruct(ptr));
		}

	public:
		AllocateType *allocate() {
			return static_cast<T *>(base_allocator_.allocate(sizeof(AllocateType)));
		}

		AllocateType *allocate(size_t num) {
			return static_cast<AllocateType *>(base_allocator_.allocate(sizeof(AllocateType) * num));
		}

		void deallocate(AllocateType *ptr) {
			base_allocator_.deallocate(ptr, sizeof(AllocateType));
		}

		void deallocate(AllocateType *ptr, size_t num) {
			base_allocator_.deallocate(ptr, sizeof(AllocateType) * num);
		}
	};

	template<class T>
		requires std::is_trivially_copyable_v<T>
	class SimpleAllocator<T> {
	public:
		using AllocateType = T;

		template<class U>
		struct Rebind {
			using type = SimpleAllocator<U>;
		};

	private:
		BaseAllocator base_allocator_;

	public:
		SimpleAllocator() = default;

		SimpleAllocator(const SimpleAllocator &other) = default;

		~SimpleAllocator() = default;

	public:
		template<class ...Args>
		AllocateType *construct(Args &&... args) {
			return util::construct<AllocateType>(allocate(sizeof(T)), std::forward<Args>(args)...);
		}

		void deconstruct(T *ptr) {
			deallocate(util::deconstruct(ptr));
		}

	public:
		AllocateType *allocate() {
			return static_cast<T *>(base_allocator_.allocate(sizeof(AllocateType)));
		}

		AllocateType *allocate(size_t num) {
			return static_cast<AllocateType *>(base_allocator_.allocate(sizeof(AllocateType) * num));
		}

		void deallocate(AllocateType *ptr) {
			base_allocator_.deallocate(ptr, sizeof(AllocateType));
		}

		void deallocate(AllocateType *ptr, size_t num) {
			base_allocator_.deallocate(ptr, sizeof(AllocateType) * num);
		}

		AllocateType *reallocate(AllocateType *ptr, size_t old_num, size_t new_num) {
			return static_cast<AllocateType *>(
			        base_allocator_.reallocate(ptr,
			                                   old_num * sizeof(AllocateType),
			                                   new_num * sizeof(T)
			                                           )
			);
		}
	};
}

#endif//ALGORITHM_ALLOCATOR_SIMPLE_ALLOCATOR_H
