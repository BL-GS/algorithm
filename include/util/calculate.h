/*
 * @author: BL-GS 
 * @date:   2023/5/12
 */

#pragma once
#ifndef ALGORITHM_UTIL_CALCULATE_H
#define ALGORITHM_UTIL_CALCULATE_H

#include <cstddef>
#include <concepts>

namespace algorithm::util {

	inline namespace func {

		template<class IntType>
		    requires std::is_integral_v<IntType>
		inline constexpr bool negtive(IntType a) {
			if constexpr (!std::is_signed_v<IntType>) { return false; }

			using UnsignedIntType = std::make_unsigned<IntType>::type;
			constexpr size_t IntTypeBits = sizeof(UnsignedIntType) * 8;
			return static_cast<UnsignedIntType>(a) >> (IntTypeBits - 1);
		}

		template<class IntType>
		    requires std::is_integral_v<IntType>
		inline constexpr bool positive(IntType a) {
			if constexpr (!std::is_signed_v<IntType>) { return true; }

			return !negtive(a);
		}

		template<class IntType>
		    requires std::is_integral_v<IntType>
		inline constexpr int sign(IntType a) {
			if constexpr (!std::is_signed_v<IntType>) {
				return static_cast<int>(a != static_cast<IntType>(0));
			}

			return static_cast<int>(a != static_cast<IntType>(0)) | static_cast<int>(negtive(a));
		}

		template<class IntType>
		    requires std::is_integral_v<IntType>
		inline constexpr IntType average(IntType a, IntType b) {
			return (a & b) + ((a ^ b) >> 1);
		}

		template<class IntType>
		    requires std::is_integral_v<IntType>
		inline constexpr IntType ceil(IntType a, size_t align) {
			return (a + align - 1) / align * align;
		}

		template<class IntType>
		    requires std::is_integral_v<IntType>
		inline constexpr IntType floor(IntType a, size_t align) {
			return a / align * align;
		}

		template<class IntType>
		    requires std::is_integral_v<IntType>
		inline constexpr IntType floor_2pow(IntType a, size_t align) {
			return a & (~(align - 1));
		}

		template<class IntType>
		    requires std::is_integral_v<IntType>
		inline constexpr IntType ceil_2pow(IntType a, size_t align) {
			return (a + align - 1) & (~(align - 1));
		}

		template<class IntType>
		    requires std::is_integral_v<IntType>
		inline constexpr bool align_to_2pow(IntType a, size_t align) {
			return (a & (align - 1)) == 0;
		}

	}

}

#endif//ALGORITHM_UTIL_CALCULATE_H
