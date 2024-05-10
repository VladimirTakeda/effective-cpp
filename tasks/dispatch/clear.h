#pragma once

template<typename T>
concept HasClear = requires(T a) {
    { a.Clear() } -> std::same_as<void>;
};

template<class T>
void Clear(T* &ptr) {
    delete ptr;
    ptr = nullptr;
}

template<class T>
void Clear(std::unique_ptr<T> &ptr) {
    ptr.reset();
}

template<class T>
void Clear(std::shared_ptr<T> &ptr) {
    ptr.reset();
}

template<HasClear T>
void Clear(T& obj) {
    obj.Clear();
}

template<class T>
void Clear(T& obj) {
}
