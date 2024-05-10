#pragma once

template <class T>
concept HasBegin = requires(T&& t) {
    ([](auto& x) -> decltype(std::begin(x)) { return std::begin(x); })(t) != nullptr;
};

template<typename T>
concept HasNonVoidBracketOperator = requires(T t, size_t index) {
    { t[index] } -> std::same_as<decltype(t[index])>;
    requires !std::is_same_v<decltype(t[index]), void>;
};

template<typename T>
concept HasRandomAccess = requires(T t) {
    requires HasBegin<T> &&
     std::same_as<typename std::iterator_traits<decltype(std::begin(t))>::iterator_category, std::random_access_iterator_tag> &&
     std::same_as<typename std::iterator_traits<decltype(std::end(t))>::iterator_category, std::random_access_iterator_tag>;
};

template <class T>
concept Indexable = requires(T t) {
    requires HasNonVoidBracketOperator<T> || HasRandomAccess<T>;
};
