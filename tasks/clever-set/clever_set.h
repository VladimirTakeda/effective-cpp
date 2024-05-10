#pragma once

#include <set>
#include <unordered_map>
#include <map>
#include <unordered_set>

template<typename T>
concept Hashable = requires(T a) {
    { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

template<typename T>
concept HasLessOperator = requires(T a, T b) {
    { a < b } -> std::same_as<bool>;
};

template<typename T>
concept HasGreaterOperator = requires(T a, T b) {
    { a > b } -> std::same_as<bool>;
};

template<typename T>
concept LessGreaterComparable = requires (T t){
    requires HasLessOperator<T> || HasGreaterOperator<T>;
};

template<typename T>
concept HasEqualOperator = requires(T a, T b) {
    { a == b } -> std::same_as<bool>;
};

template<typename T>
concept HasNonEqualOperator = requires(T a, T b) {
    { a != b } -> std::same_as<bool>;
};

template<typename T>
concept EqualityComparable = requires(T a) {
    requires HasEqualOperator<T> || HasNonEqualOperator<T> || LessGreaterComparable<T>;
};



template<typename T>
bool isEqual(const T& a, const T& b) requires (HasEqualOperator<T>) {
    return a == b;
}

template<typename T>
bool isEqual(const T& a, const T& b) requires (! HasEqualOperator<T> && HasNonEqualOperator<T>) {
    return !(a != b);
}

template<typename T>
bool isEqual(const T& a, const T& b) requires (!HasEqualOperator<T> && HasLessOperator<T> && !HasGreaterOperator<T>) {
    return (a < b) && (b < a);
}

template<typename T>
bool isEqual(const T& a, const T& b) requires (!HasEqualOperator<T> && !HasLessOperator<T> && HasGreaterOperator<T>) {
    return !(a > b) && !(b > a);
}

template<typename T>
struct Int64PtrEqual {
    bool operator()(const T& lhs, const T& rhs) const {
        return isEqual(lhs, rhs);
    }
};

template<class T, typename A=std::unordered_set<T, std::hash<T>, Int64PtrEqual<T>>, typename B=std::set<T>, typename U=std::unordered_set<const T*>>
class CleverSet {
public:
    bool insert(const T &value) {
        if constexpr(std::is_pointer_v<typename decltype(data_)::value_type>) {
            return data_.insert(&value).second;
        } else {
            return data_.insert(value).second;
        }
    }

    bool erase(const T &value) {
        if constexpr(std::is_pointer_v<typename decltype(data_)::value_type>) {
            return data_.erase(&value);
        } else {
            return data_.erase(value);
        }
    }

    bool find(const T &value) const {
        if constexpr(std::is_pointer_v<typename decltype(data_)::value_type>) {
            return data_.find(&value) != data_.end();
        }
        else {
            return data_.find(value) != data_.end();
        }
    }

    size_t size() const {
        return data_.size();
    }

private:
    std::conditional_t<Hashable<T> && EqualityComparable<T>, A,
            std::conditional_t<LessGreaterComparable<T>, B, U>> data_;
};
