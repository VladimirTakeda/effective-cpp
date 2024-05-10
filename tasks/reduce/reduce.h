#pragma once

#include <iostream>
#include <vector>
#include <thread>
#include <functional>
#include <numeric>

template <class RandomAccessIterator, class T, class Func>
void ReduceChunk(RandomAccessIterator first, RandomAccessIterator last, T& result, const T& initial, Func func) {
    result = std::accumulate(first, last, initial, func);
}

template <class RandomAccessIterator, class T, class Func>
T Reduce(RandomAccessIterator first, RandomAccessIterator last, const T& initial_value, Func func) {
    if (first == last)
        return initial_value;
    size_t n = std::distance(first, last);
    size_t num_threads = std::thread::hardware_concurrency() / 2;
    size_t chunk_size = (n + num_threads - 1) / num_threads;

    std::vector<std::thread> threads;
    std::vector<T> results(num_threads, initial_value);

    for (size_t i = 0; i < num_threads; ++i) {
        RandomAccessIterator start = first + i * chunk_size;
        RandomAccessIterator end = first + std::min((i + 1) * chunk_size, n);
        threads.push_back(std::thread(ReduceChunk<RandomAccessIterator, T, Func>, start, end, std::ref(results[i]), initial_value, func));
        if (end == last)
            break;
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return std::accumulate(results.begin(), results.end(), initial_value, func);
}
