/*
 * @author: BL-GS 
 * @date:   2023/5/15
 */

#pragma once
#ifndef ALGORITHM_ALGORITHM_SORT_INSERT_SORT_H
#define ALGORITHM_ALGORITHM_SORT_INSERT_SORT_H

#include <util/compare.h>

namespace algorithm {

	inline namespace sort {

		template<class Iterator, class Cmp>
		inline void insert_sort(Iterator start_iter, Iterator end_iter, Cmp cmp_func) {
			for (Iterator iter = start_iter; iter != end_iter; ++iter) {
				Iterator ultimate_iter = util::func::argidx(iter, end_iter, cmp_func);
				if (iter != ultimate_iter) {
					std::swap(*iter, *ultimate_iter);
				}
			}
		}

		template<class Iterator>
		inline void insert_sort(Iterator start_iter, Iterator end_iter) {
			for (Iterator iter = start_iter; iter != end_iter; ++iter) {
				Iterator ultimate_iter = util::func::argmin(iter, end_iter);
				if (iter != ultimate_iter) {
					std::swap(*iter, *ultimate_iter);
				}
			}
		}
	}

}

#endif//ALGORITHM_ALGORITHM_SORT_INSERT_SORT_H
