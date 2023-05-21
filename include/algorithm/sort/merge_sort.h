/*
 * @author: BL-GS 
 * @date:   2023/5/15
 */

#pragma once
#ifndef ALGORITHM_ALGORITHM_SORT_MERGE_SORT_H
#define ALGORITHM_ALGORITHM_SORT_MERGE_SORT_H

#include <iterator>

namespace algorithm {

	inline namespace sort {

		template<class Iterator, class Cmp>
		inline void merge_two_array(Iterator first_start_iter, Iterator first_end_iter,
		                            Iterator second_start_iter, Iterator second_end_iter,
		                            Iterator target_start_iter,
		                            Cmp cmp_func) {
			while (first_start_iter != first_end_iter && second_start_iter != second_end_iter) {
				if (cmp_func(*first_start_iter, *second_start_iter)) {
					*target_start_iter = *first_start_iter;
					++first_start_iter;
				}
				else {
					*target_start_iter = *second_start_iter;
					++second_start_iter;
				}
				++target_start_iter;
			}
			while (first_start_iter != first_end_iter) {
				*target_start_iter = *first_start_iter;
				++target_start_iter;
				++first_start_iter;
			}
			while (second_start_iter != second_end_iter) {
				*target_start_iter = *second_start_iter;
				++target_start_iter;
				++second_start_iter;
			}
		}

		template<class Iterator, class Cmp>
		inline void merge_two_array_move(Iterator first_start_iter, Iterator first_end_iter,
		                                 Iterator second_start_iter, Iterator second_end_iter,
		                                 Iterator target_start_iter,
		                                 Cmp cmp_func) {
			while (first_start_iter != first_end_iter && second_start_iter != second_end_iter) {
				if (cmp_func(*first_start_iter, *second_start_iter)) {
					*target_start_iter = std::move(*first_start_iter);
					++first_start_iter;
				}
				else {
					*target_start_iter = std::move(*second_start_iter);
					++second_start_iter;
				}
				++target_start_iter;
			}
			while (first_start_iter != first_end_iter) {
				*target_start_iter = std::move(*first_start_iter);
				++target_start_iter;
				++first_start_iter;
			}
			while (second_start_iter != second_end_iter) {
				*target_start_iter = std::move(*second_start_iter);
				++target_start_iter;
				++second_start_iter;
			}
		}

	}

}

#endif//ALGORITHM_ALGORITHM_SORT_MERGE_SORT_H
