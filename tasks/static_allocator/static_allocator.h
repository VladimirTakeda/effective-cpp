#pragma once

#include <stdexcept>
#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

template<class T>
class StaticAllocator {
private:
    T* buffer = nullptr;
    size_t count_;

public:
    explicit StaticAllocator(size_t count) {
        void* ptr = std::aligned_alloc(alignof(T) < 8 ? 8 : alignof(T), count * sizeof(T));
        buffer = reinterpret_cast<T*>(ptr);
        if (buffer == nullptr)
            throw std::bad_alloc();
        busy = std::vector<bool>(count);
        freeIndex.reserve(count);
        freeIndex.resize(count);
        std::iota(freeIndex.begin(), freeIndex.end(), 0);
        count_ = count;
    }

    ~StaticAllocator() {
        T* begin = buffer;
        for (size_t i = 0; i < count_; ++i){
            if (busy[i])
                begin->~T();
            ++begin;
        }
        std::free(buffer);
    }

    template<class... Args>
    T* New(Args&&... args) {
        if (freeIndex.empty())
            return nullptr;
        size_t index = freeIndex.back();
        freeIndex.pop_back();
        busy[index] = true;
        T* obj = new (buffer + index) T(std::forward<Args>(args)...);
        return obj;
    }

    void Delete(T *obj) {
        if (obj == nullptr)
            return;
        size_t index = obj - buffer;
        freeIndex.push_back(index);
        busy[index] = false;
        obj->~T();
    }

    std::vector<size_t> freeIndex;
    std::vector<bool> busy;
};
