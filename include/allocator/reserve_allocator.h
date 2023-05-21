/*
 * @author: BL-GS 
 * @date:   2023/5/13
 */

#pragma once
#ifndef ALGORITHM_ALLOCATOR_RESERVE_ALLOCATOR_H
#define ALGORITHM_ALLOCATOR_RESERVE_ALLOCATOR_H

#include <cstddef>
#include <cstdlib>
#include <utility>
#include <concepts>

#include <util/func/type.h>
#include <allocator/thread_allocaotr.h>

namespace algorithm::allocator {

	template<class T>
	class ReserveAllocator {
	public:
		using AllocateType = T;

		template<class U>
		struct Rebind {
			using type = ReserveAllocator<U>;
		};

	public:
		ReserveAllocator() = default;

		ReserveAllocator(const ReserveAllocator &other) = default;

		~ReserveAllocator() = default;

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
			return static_cast<T *>(get_base_allocator().allocate(sizeof(AllocateType)));
		}

		AllocateType *allocate(size_t num) {
			return static_cast<AllocateType *>(get_base_allocator().allocate(sizeof(AllocateType) * num));
		}

		void deallocate(AllocateType *ptr) {
			get_base_allocator().deallocate(ptr, sizeof(AllocateType));
		}

		void deallocate(AllocateType *ptr, size_t num) {
			get_base_allocator().deallocate(ptr, sizeof(AllocateType) * num);
		}

	private:
		static ReserveAllocatorPool &get_base_allocator() {
			return thread_allocator_pool;
		}
	};

	template<class T>
		requires std::is_trivially_copyable_v<T>
	class ReserveAllocator<T> {
	public:
		using AllocateType = T;

		template<class U>
		struct Rebind {
			using type = ReserveAllocator<U>;
		};

	public:
		ReserveAllocator() = default;

		ReserveAllocator(const ReserveAllocator &other) = default;

		~ReserveAllocator() = default;

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
			return static_cast<T *>(get_base_allocator().allocate(sizeof(AllocateType)));
		}

		AllocateType *allocate(size_t num) {
			return static_cast<AllocateType *>(get_base_allocator().allocate(sizeof(AllocateType) * num));
		}

		void deallocate(AllocateType *ptr) {
			get_base_allocator().deallocate(ptr, sizeof(AllocateType));
		}

		void deallocate(AllocateType *ptr, size_t num) {
			get_base_allocator().deallocate(ptr, sizeof(AllocateType) * num);
		}

		AllocateType *reallocate(AllocateType *ptr, size_t old_num, size_t new_num) {
			return static_cast<AllocateType *>(
			        get_base_allocator().reallocate(ptr,
			                                   old_num * sizeof(AllocateType),
			                                   new_num * sizeof(T)
                   )
			);
		}

	private:
		static ReserveAllocatorPool &get_base_allocator() {
			return thread_allocator_pool;
		}
	};

}

#endif//ALGORITHM_ALLOCATOR_RESERVE_ALLOCATOR_H
