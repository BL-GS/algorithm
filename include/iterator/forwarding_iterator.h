/*
 * @author: BL-GS 
 * @date:   2023/5/3
 */

#pragma once
#ifndef ALGORITHM_ITERATOR_FORWARDING_ITERATOR_H
#define ALGORITHM_ITERATOR_FORWARDING_ITERATOR_H

#include <concepts>
#include <iterator/abstract_iterator.h>

namespace algorithm::iterator {

    template<class ForwardingIter>
    concept ForwardingIterConcept = requires(ForwardingIter iter) {

        requires AbstractIterConcept<ForwardingIter>;

        { iter++ } -> std::same_as<ForwardingIter>;

        { ++iter } -> std::same_as<ForwardingIter &>;
    };

}


#endif//ALGORITHM_ITERATOR_FORWARDING_ITERATOR_H