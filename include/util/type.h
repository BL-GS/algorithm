/*
 * @author: BL-GS 
 * @date:   2023/5/12
 */

#pragma once
#ifndef ALGORITHM_UTIL_TYPE_H
#define ALGORITHM_UTIL_TYPE_H

#include <utility>

namespace algorithm::util {

	inline namespace func {

		template<class T, class ...Args>
		inline T *construct(void *ptr, Args &&...args) {
			T *type_ptr = static_cast<T *>(ptr);
			new (type_ptr) T(std::forward<Args>(args)...);
			return type_ptr;
		}

		template<class T>
		inline T *deconstruct(T *ptr) {
			ptr->~T();
			return ptr;
		}

	}

}

#endif//ALGORITHM_UTIL_TYPE_H
