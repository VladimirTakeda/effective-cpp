#pragma once

#include <iostream>
#include <vector>

using namespace std;

template<class T>
class ReverseVector {
public:
    explicit ReverseVector(std::vector<T>& data) : data_(data) {
        size = data.size();
        uint64_t* dataPointer = reinterpret_cast<uint64_t*>(&data_);
        dataPointer[0] += sizeof(T) * size;
        dataPointer[1] += sizeof(T) * size;
    }

    ~ReverseVector() {
        uint64_t* dataPointer = reinterpret_cast<uint64_t*>(&data_);
        dataPointer[0] -= sizeof(T) * size;
        dataPointer[1] -= sizeof(T) * size;
    }

private:
    size_t size = 0;
    std::vector<T>& data_;
};
