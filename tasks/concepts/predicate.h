#pragma once

template <class P, class T>
concept Predicate = requires(P func, T a){
    {func(a)} -> std::same_as<bool>;
};