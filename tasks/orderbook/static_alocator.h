#pragma once

#include <set>
#include <cstddef>
#include <iostream>
#include <algorithm>
#include <numeric>

using namespace std;

template <class T>
class StaticStlAllocator : public std::allocator<T> {
public:
    explicit StaticStlAllocator(size_t count) : count_(count){
        Initilize();
    }

    void Initilize(){
        void* ptr = std::aligned_alloc(alignof(T) < 8 ? 8 : alignof(T), count_ * sizeof(T));
        mempool_ = reinterpret_cast<char *>(ptr);
        if (mempool_ == nullptr)
            throw std::bad_alloc();
        busy = std::vector<bool>(count_);
        freeIndex.reserve(count_);
        freeIndex.resize(count_);
        std::iota(freeIndex.begin(), freeIndex.end(), 0);
    }

    using value_type = T;

    StaticStlAllocator(StaticStlAllocator&& other) noexcept
            : count_(other.count_), mempool_(other.mempool_), freeIndex(std::move(other.freeIndex)), busy(std::move(other.busy)) {
        other.mempool_ = nullptr;
        other.count_ = 0;
    }

    StaticStlAllocator(const StaticStlAllocator& other){
        count_ = other.count_;
        Initilize();
    }
    StaticStlAllocator& operator=(const StaticStlAllocator&) = default;

    template <class U>
    struct rebind {
        using other = StaticStlAllocator<U>;
    };

    template <class U>
    StaticStlAllocator(const StaticStlAllocator<U>& other){
        count_ = other.count_;
        Initilize();
    }

    T *allocate(size_t count){
        if (count != 1)
            throw std::bad_alloc();

        if (freeIndex.empty())
            return nullptr;
        size_t index = freeIndex.back();
        freeIndex.pop_back();
        busy[index] = true;
        return reinterpret_cast<T*>(mempool_) + index;
    }

    // Do nothing for deallocation (memory is not freed)
    void deallocate(T* ptr, size_t count) {
        size_t index = ptr - reinterpret_cast<T*>(mempool_);
        for (int i = 0; i < count; ++i){
            freeIndex.push_back(index);
            busy[index] = false;
            ++index;
        }
    }

    // Construct an object at the given address
    template<class U, class... Args>
    void construct(U* p, Args&&... args) {
        ::new (static_cast<void*>(p)) U(std::forward<Args>(args)...);
    }

    // Destroy an object at the given address
    template<class U>
    void destroy(U* p) {
        p->~U();
    }

    char *mempool_ = nullptr;
    size_t count_ = 0;

    std::vector<size_t> freeIndex;
    std::vector<bool> busy;
};