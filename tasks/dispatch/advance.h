#pragma once

#include <cstddef>
#include <iterator>
#include <vector>

namespace implementation_details{
    template<class Iterator>
    void Advance(Iterator& iterator, int n, std::input_iterator_tag) {
        for (int i = 0; i < n; ++i)
            ++iterator;
    }

    template<class Iterator>
    void Advance(Iterator& iterator, int n, std::output_iterator_tag) {
        for (int i = 0; i < n; ++i)
            ++iterator;
    }

    template<class Iterator>
    void Advance(Iterator& iterator, int n, std::bidirectional_iterator_tag) {
        if (n > 0){
            for (int i = 0; i < n; ++i){
                ++iterator;
            }
        }
        else{
            for (int i = 0; i < abs(n); ++i){
                --iterator;
            }
        }
    }

    template<class Iterator>
    void Advance(Iterator& iterator, int n, std::forward_iterator_tag) {
        for (int i = 0; i < n; ++i)
            ++iterator;
    }

    template<class Iterator>
    void Advance(Iterator& iterator, int n, std::random_access_iterator_tag) {
        iterator += n;
    }
}

template<class Iter>
void Advance(Iter& first, int count)
{
    implementation_details::Advance(first, count, typename std::iterator_traits<Iter>::iterator_category());
}

