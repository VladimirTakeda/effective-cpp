#pragma once

#include <set>
#include <cstddef>
#include <iostream>

using namespace std;

template <class T>
class StaticStlAllocator : public std::allocator<T> {
public:
    explicit StaticStlAllocator(char *mempool, size_t size) : mempool_(mempool), size_(size), curr_(mempool_) {
    }

    StaticStlAllocator(const StaticStlAllocator&) = default;
    StaticStlAllocator& operator=(const StaticStlAllocator&) = default;

    template <class U>
    struct rebind {
        using other = StaticStlAllocator<U>;
    };

    template <class U>
    StaticStlAllocator(const StaticStlAllocator<U>& other) : mempool_(other.mempool_), size_(other.size_), curr_(other.curr_) {}

    T *allocate(size_t count){
        if (curr_ + count * sizeof(T) > mempool_ + size_) {
            throw std::bad_alloc();
        }
        char *ptr = curr_;
        curr_ += count * sizeof(T);
        return reinterpret_cast<T*>(ptr);
    }

    // Do nothing for deallocation (memory is not freed)
    void deallocate(T* /*ptr*/, size_t /*count*/) {}

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
    size_t size_ = 0;
    char* curr_ = nullptr;
};

// Define a set using the custom allocator
template<class T, class Compare = std::less<T>>
using Set = std::set<T, Compare, StaticStlAllocator<T>>;