#pragma once

#include <stdexcept>

template<class T>
inline T ChangeByteOrder(T value) {
    int size = sizeof(T);
    for (int i = 0; i < size/2; ++i){
        std::swap(*(reinterpret_cast<char*>(&value) + i), *(reinterpret_cast<char*>(&value) + size - 1 - i));
    }
    return value;
}
