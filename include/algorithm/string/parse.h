/*
 * @author: BL-GS 
 * @date:   2023/5/15
 */

#pragma once
#ifndef ALGORITHM_ALGORITHM_STRING_PARSE_H
#define ALGORITHM_ALGORITHM_STRING_PARSE_H

#include <concepts>
#include <cstddef>
#include <cstring>
#include <limits>

namespace algorithm {

	inline namespace string {

		enum class Radix : int {
			Binary = 2,
			Decimal = 10,
			Hexadecimal = 16
		};

		inline char RADIX_CHAR[] = {
		        '0', '1', '2', '3', '4',
		        '5', '6', '7', '8', '9',
		        'A', 'B', 'C', 'D', 'E', 'F'};

		template<class IntType>
		    requires std::is_integral_v<IntType>
		inline consteval size_t max_char_from_integer(Radix radix) {
			IntType radix_value = static_cast<IntType>(radix);

			size_t max_number_length = 0;
			size_t min_number_length = 0;

			IntType max_number = std::numeric_limits<IntType>::max();
			while (max_number != 0) {
				max_number /= radix_value;
				++max_number_length;
			}

			IntType min_number = std::numeric_limits<IntType>::min();
			if (min_number < 0) { ++min_number_length; }
			while (min_number != 0) {
				min_number /= radix_value;
				++min_number_length;
			}

			return (max_number_length > min_number_length) ? max_number_length : min_number_length;
		}

		template<Radix radix, class IntType>
		    requires std::is_integral_v<IntType> && (!std::is_signed_v<IntType>)
		inline constexpr size_t parse_integer(IntType number, char *buffer_ptr) {
			constexpr IntType radix_value = static_cast<IntType>(radix);
			constexpr size_t max_length = max_char_from_integer<IntType>(radix);
			char temp_buffer[max_length];

			size_t reverse_length = max_length;
			while (number != 0) {
				--reverse_length;
				temp_buffer[reverse_length] = RADIX_CHAR[number % radix_value];
				number /= radix_value;
			}
			size_t length = max_length - reverse_length;

			if (length == 0) {
				buffer_ptr[0] = RADIX_CHAR[0];
				return 1;
			}
			std::memcpy(buffer_ptr, temp_buffer + reverse_length, length);
			return length;
		}

		template<Radix radix, class IntType>
		    requires std::is_integral_v<IntType> && std::is_signed_v<IntType>
		inline constexpr size_t parse_integer(IntType number, char *buffer_ptr) {
			constexpr IntType radix_value = static_cast<IntType>(radix);
			constexpr size_t max_length = max_char_from_integer<IntType>(radix);
			char temp_buffer[max_length];

			bool negative = number < 0;

			size_t reverse_length = max_length;
			if (negative) {
				while (number != 0) {
					--reverse_length;
					temp_buffer[reverse_length] = RADIX_CHAR[-(number % radix_value)];
					number /= radix_value;
				}
				temp_buffer[--reverse_length] = '-';
			}
			else {
				while (number != 0) {
					--reverse_length;
					temp_buffer[reverse_length] = RADIX_CHAR[number % radix_value];
					number /= radix_value;
				}
			}

			size_t length = max_length - reverse_length;

			if (length == 0) {
				buffer_ptr[0] = RADIX_CHAR[0];
				return 1;
			}
			std::memcpy(buffer_ptr, temp_buffer + reverse_length, length);
			return length;
		}

	}

}

#endif//ALGORITHM_ALGORITHM_STRING_PARSE_H
