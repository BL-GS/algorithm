/*
 * @author: BL-GS 
 * @date:   2023/5/22
 */

#pragma once
#ifndef ALGORITHM_MEMORY_CACHE_H
#define ALGORITHM_MEMORY_CACHE_H

#include <cstddef>

namespace algorithm::memory {

	// It is OK on X86-64
	static constexpr size_t CACHE_LINE_SIZE           = 64;
	// Minimum offset between two objects in case of faulty share
	static constexpr size_t CACHE_LINE_SIZE_CONSTRUCT = CACHE_LINE_SIZE;
	// Maximum size of contiguous memory to promote true sharing.
	static constexpr size_t CACHE_LINE_SIZE_DESTRUCT  = CACHE_LINE_SIZE;

}

#endif//ALGORITHM_MEMORY_CACHE_H
