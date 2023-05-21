/*
 * @author: BL-GS 
 * @date:   2023/5/3
 */

#pragma once
#ifndef ALGORITHM_ALLOCATOR_BASE_ALLOCATOR_H
#define ALGORITHM_ALLOCATOR_BASE_ALLOCATOR_H

#include <cstddef>
#include <cstdlib>
#include <utility>

#include <util/func/type.h>

namespace algorithm::allocator {

	class BaseAllocator {
	public:
		BaseAllocator() = default;

		BaseAllocator(const BaseAllocator &other) = default;

		~BaseAllocator() = default;

	public:
		static void *allocate(size_t size) {
			return std::malloc(size);
		}

		static void deallocate(void *ptr, [[maybe_unused]] size_t size) {
			std::free(ptr);
		}

		static void *reallocate(void *ptr, [[maybe_unused]] size_t old_size, size_t new_size) {
			return std::realloc(ptr, new_size);
		}
	};
}

#endif//ALGORITHM_ALLOCATOR_BASE_ALLOCATOR_H