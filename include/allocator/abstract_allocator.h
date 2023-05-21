/*
 * @author: BL-GS 
 * @date:   2023/5/3
 */

#pragma once
#ifndef ALGORITHM_ALLOCATOR_ABSTRACT_ALLOCATOR_H
#define ALGORITHM_ALLOCATOR_ABSTRACT_ALLOCATOR_H

#include <concepts>
#include <cstddef>

namespace algorithm::allocator {

	template<class Allocator>
	concept AllocatorConcept = requires(
        Allocator alloc,
        Allocator::template Rebind<int>::type rebind_alloc,
        Allocator::AllocateType value,
        Allocator::AllocateType *ptr,
        size_t num) {

        { alloc.construct(value) } -> std::same_as<typename Allocator::AllocateType *>;

        { alloc.deconstruct(ptr) } -> std::same_as<void>;

        { alloc.allocate() } -> std::same_as<typename Allocator::AllocateType *>;

		{ alloc.allocate(num) } -> std::same_as<typename Allocator::AllocateType *>;

        { alloc.deallocate(ptr) } -> std::same_as<void>;

		{ alloc.deallocate(ptr, num) } -> std::same_as<void>;

		{ alloc.reallocate(ptr, num, num) } -> std::same_as<typename Allocator::AllocateType *>;

	};
}

#endif//ALGORITHM_ALLOCATOR_ABSTRACT_ALLOCATOR_H