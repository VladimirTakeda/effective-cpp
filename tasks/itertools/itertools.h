#pragma once

#include <iostream>

template <class Iterator>
class IteratorRange;

template <class Iterator>
class IteratorRange {
public:
    using value_type = Iterator::value_type;
    using iterator = Iterator;
    IteratorRange(Iterator begin, Iterator end) : begin_(begin), end_(end) {
    }

    Iterator begin() const {
        return begin_;
    }

    Iterator end() const {
        return end_;
    }

private:
    Iterator begin_, end_;
};

template <class T>
class NumericIterator {
public:
    using value_type = T;

    explicit NumericIterator(T number = 0, int step = 1) : value_(number), step_(step) {}

    NumericIterator& operator++() {
        value_ += step_;
        return *this;
    }

    const T& operator*() const { return value_; }
    T& operator*() { return value_; }

    T value_;
    T step_;
};

template <class T>
bool operator==(const NumericIterator<T>& lhs, const NumericIterator<T>& rhs) {
    return lhs.value_ >= rhs.value_;
}

template<class FirstRange, class SecondRange>
class PairIterator {
public:
    using value_type = std::pair<typename std::decay<FirstRange>::type::value_type, typename std::decay<SecondRange>::type::value_type>;

    explicit PairIterator(typename std::decay<FirstRange>::type::iterator first, typename std::decay<SecondRange>::type::iterator second) : firstIter(first), secondIter(second) {}

    PairIterator& operator++() {
        ++firstIter;
        ++secondIter;
        return *this;
    }

    value_type operator*() const { return std::make_pair(*firstIter, *secondIter); }
    value_type operator*() { return std::make_pair(*firstIter, *secondIter); }

    typename std::decay<FirstRange>::type::iterator firstIter;
    typename std::decay<SecondRange>::type::iterator secondIter;
};

template<class IteratorRange>
class SameIterator{
public:

    using value_type = typename std::decay<IteratorRange>::type::value_type;

    explicit SameIterator(typename std::decay<IteratorRange>::type::iterator first) : firstIter(first){
        std::cout << "type " << typeid(IteratorRange).name() << std::endl;
        std::cout << "type " << typeid(value_type).name() << std::endl;
        std::cout << "constructor: " << &(*firstIter) << std::endl;
    }

    SameIterator& operator++() {
        std::cout << "inc: " << &(*firstIter) << std::endl;
        ++firstIter;
        return *this;
    }

    value_type operator*() const {
        std::cout << "value 2: " << &(*firstIter) << std::endl;
        return *firstIter;
    }
    value_type operator*() {
        std::cout << "value: " << &(*firstIter) << std::endl;
        return *firstIter;
    }

    typename std::decay<IteratorRange>::type::iterator firstIter;
};

template <class T>
bool operator==(const SameIterator<T>& lhs, const SameIterator<T>& rhs) {
    return lhs.firstIter == rhs.firstIter;
}

template <typename RangeType>
class GroupIterator {
public:

    using value_type = typename std::decay<RangeType>::type::value_type;

    explicit GroupIterator(typename std::decay<RangeType>::type::iterator begin, typename std::decay<RangeType>::type::iterator end) : current_(begin), next_(begin), end_(end) {
        while (next_!= end_ && *current_ == *next_){
            std::cout << 1 << std::endl;
            ++next_;
        }
    }

    bool operator!=(const GroupIterator& other) const {
        return current_ != other.current_;
    }

    IteratorRange<SameIterator<RangeType>> operator*() const {
        std::cout << "*************************" << std::endl;
        std::cout << &(*current_) << " " << &(*next_) << std::endl;
        std::cout << std::distance(current_, next_) << std::endl;
        std::cout << "*************************" << std::endl;
        return IteratorRange<SameIterator<RangeType>>(SameIterator<RangeType>(current_), SameIterator<RangeType>(next_));
    }

    GroupIterator& operator++() {
        if (current_ != end_) {
            current_ = next_;
            while (next_!= end_ && *current_ == *next_){
                ++next_;
            }
        }
        return *this;
    }

    IteratorRange<SameIterator<RangeType>> begin() const {
        std::cout << "_________________________" << std::endl;
        std::cout << &(*current_) << " " << &(*next_) << std::endl;
        std::cout << std::distance(current_, next_) << std::endl;
        std::cout << "_________________________" << std::endl;
        return IteratorRange<SameIterator<RangeType>>(SameIterator<RangeType>(current_), SameIterator<RangeType>(next_));;
    }


    typename std::decay<RangeType>::type::iterator current_;
    typename std::decay<RangeType>::type::iterator next_;
    typename std::decay<RangeType>::type::iterator end_;
};

template <class T>
bool operator==(const GroupIterator<T>& lhs, const GroupIterator<T>& rhs) {
    return lhs.current_ == rhs.current_;
}


template<class T, class V>
bool operator==(const PairIterator<T, V>& lhs, const PairIterator<T, V>& rhs) {
    return lhs.firstIter == rhs.firstIter;
}

template <class T>
IteratorRange<NumericIterator<T>> Range(T number){
    return IteratorRange<NumericIterator<T>>(NumericIterator<T>(0), NumericIterator<T>(number));
}

template <class T>
IteratorRange<NumericIterator<T>> Range(T from, T to, T step){
    return IteratorRange<NumericIterator<T>>(NumericIterator<T>(from, step), NumericIterator<T>(to));
}

template <class T>
IteratorRange<NumericIterator<T>> Range(T from, T to){
    return IteratorRange<NumericIterator<T>>(NumericIterator<T>(from), NumericIterator<T>(to));
}

template <class T, class V>
IteratorRange<PairIterator<T, V>> Zip(T&& firstRange, V&& secondRange){
    auto start = PairIterator<T, V>(firstRange.begin(), secondRange.begin());
    auto end = PairIterator<T, V>(firstRange.end(), secondRange.end());
    return IteratorRange<PairIterator<T, V>>(start, end);
}


// разобраться почему нужно передавать через &&
template <class T>
IteratorRange<GroupIterator<T>> Group(T&& container){
    return IteratorRange<GroupIterator<T>>(GroupIterator<T>(container.begin(), container.end()), GroupIterator<T>(container.end(), container.end()));
}
