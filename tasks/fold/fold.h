#pragma once

struct Sum {
    template <typename T>
    T operator()(const T& a, const T& b) const {
        return a + b;
    }
};

struct Prod {
    template <typename T>
    T operator()(const T& a, const T& b) const {
        return a * b;
    }
};

struct Concat {
    template <typename T>
    std::vector<T> operator()(const std::vector<T>& a, const std::vector<T>& b) const {
        std::vector<T> result = a;
        result.reserve(result.size() + b.size());
        result.insert(result.end(), b.begin(), b.end());
        return result;
    }
};


class Length {
public:
    explicit Length(int* len_ptr) : len_ptr_(len_ptr) {}

    template <typename N, typename T>
    N operator()(const N& a, const T& b) const {
        ++(*len_ptr_);
        return a;
    }

private:
    int* len_ptr_;
};

template <class Iterator, class T, class BinaryOp>
T Fold(Iterator first, Iterator last, T init, BinaryOp func) {
    for (; first != last; ++first) {
        init = func(init, *first);
    }
    return init;
}
