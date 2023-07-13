/*
 * @author: BL-GS 
 * @date:   2023/5/26
 */

#pragma once
#ifndef ALGORITHM_MEMORY_NVM_H
#define ALGORITHM_MEMORY_NVM_H

#include <cstdint>
#include <thread>
#include <immintrin.h>

#include <memory/cache.h>

namespace algorithm::memory {

	#ifndef PWB_DEFINED
		#define PWB_DEFINED CLWB
	#endif
	#define PWB_ENUM Flush::PWB_DEFINED

	enum class Flush {
		CLWB,
		CLFLUSH,
		CLFLUSHOPT
	};

	inline Flush get_current_pwb_type() {
		return PWB_ENUM;
	}

	inline void clwb(void *target) {
		_mm_clwb(target);
	}

	inline void clflush(void *target) {
		_mm_clflush(target);
	}

	inline void clflushopt(void *target) {
		_mm_clflushopt(target);
	}

	inline void clwb_range(void *start_ptr, uint32_t size) {
		uint8_t *target = static_cast<uint8_t *>(start_ptr);
		for (uint32_t i = 0; i < size; i += CACHE_LINE_SIZE) {
			_mm_clwb(target + i);
		}
	}

	inline void clflush_range(void *start_ptr, uint32_t size) {
		uint8_t *target = static_cast<uint8_t *>(start_ptr);
		for (uint32_t i = 0; i < size; i += CACHE_LINE_SIZE) {
			_mm_clflush(target + i);
		}
	}

	inline void clflushopt_range(void *start_ptr, uint32_t size) {
		uint8_t *target = static_cast<uint8_t *>(start_ptr);
		for (uint32_t i = 0; i < size; i += CACHE_LINE_SIZE) {
			_mm_clflushopt(target + i);
		}
	}

	template<Flush FlushType = PWB_ENUM>
	struct NVMConfig {};

	template<>
	struct NVMConfig<Flush::CLWB> {

		static inline void pwb(void *target) {
			clwb(target);
		}

		static inline void pwb_range(void *start_ptr, uint32_t size) {
			clwb_range(start_ptr, size);
		}

		static inline void fence() {
			asm volatile("sfence\n" : :);
		}

		static inline void fence_other() {
			asm volatile("sfence\n" : :);
		}
	};

	template<>
	struct NVMConfig<Flush::CLFLUSH> {

		static inline void pwb(void *target) {
			clflush(target);
		}

		static inline void pwb_range(void *start_ptr, uint32_t size) {
			clflush_range(start_ptr, size);
		}

		static inline void fence() { }

		static inline void fence_other() {
			asm volatile("sfence\n" : :);
		}
	};

	template<>
	struct NVMConfig<Flush::CLFLUSHOPT> {

		static inline void pwb(void *target) {
			clflushopt(target);
		}

		static inline void pwb_range(void *start_ptr, uint32_t size) {
			clflushopt_range(start_ptr, size);
		}

		static inline void fence() {
			asm volatile("sfence\n" : :);
		}

		static inline void fence_other() {
			asm volatile("sfence\n" : :);
		}
	};

	using NVM = NVMConfig<PWB_ENUM>;
}

#endif//ALGORITHM_MEMORY_NVM_H
