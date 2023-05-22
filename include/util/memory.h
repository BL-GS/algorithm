/*
 * @author: BL-GS 
 * @date:   2023/5/12
 */

#pragma once
#ifndef ALGORITHM_UTIL_MEMORY_H
#define ALGORITHM_UTIL_MEMORY_H

inline static constexpr unsigned long long operator""_bits(unsigned long long num) {
	return (num + 7) / 8;
}

inline static constexpr unsigned long long operator""_B(unsigned long long num) {
	return num;
}

inline static constexpr unsigned long long operator""_KB(unsigned long long num) {
	return num * 1024;
}

inline static constexpr unsigned long long operator""_MB(unsigned long long num) {
	return num * 1024 * 1024;
}

inline static constexpr unsigned long long operator""_GB(unsigned long long num) {
	return num * 1024 * 1024 * 1024;
}

#endif//ALGORITHM_UTIL_MEMORY_H
