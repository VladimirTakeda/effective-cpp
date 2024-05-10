#pragma once

#include <algorithm>

class ListHook;
class Base{
public:
    Base(ListHook* head = nullptr, ListHook* tail = nullptr, size_t size = 0) : size_(size), head_(head), tail_(tail){}
public:
    size_t size_;
    ListHook *head_;
    ListHook *tail_;
};

class ListHook {
public:
    ListHook() : next_(nullptr), isLinked_(){}

    //in list or not
    [[nodiscard]] bool IsLinked() const{
        return isLinked_;
    }

    // remove from the list
    void Unlink(){
        if (isLinked_) {
            if (list && this == list->head_)
                list->head_ = list->head_->next_;
            if (list && this == list->tail_)
                list->tail_ = list->tail_->prev_;
            if (prev_)
                prev_->next_ = next_;
            if (next_)
                next_->prev_ = prev_;
            if (list)
                --(list->size_);
            list = nullptr;
            prev_ = nullptr;
            next_ = nullptr;
            isLinked_ = false;
        }
    }

    // Must unlink element from the list
    ~ListHook(){
        Unlink();
    }

    ListHook(const ListHook&) = delete;

private:
    template <class T>
    friend class List;

    Base* list = nullptr;
    ListHook* next_ = nullptr;
    ListHook* prev_ = nullptr;
    bool isLinked_;
};

template <typename T>
class List : public Base{
public:
    class Iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
    public:
        explicit Iterator(T* ptr) : ptr_(ptr) {}

        Iterator& operator++(){
            ptr_ = static_cast<T*>(static_cast<void*>(ptr_->next_));
            return *this;
        }
        Iterator operator++(int){
            Iterator temp = *this;
            ptr_ = static_cast<T*>(static_cast<void*>(ptr_->next_));
            return temp;
        }

        T& operator*() const{
            return *ptr_;
        }
        T* operator->() const{
            return ptr_;
        }

        bool operator==(const Iterator& rhs) const{
            return ptr_ == rhs.ptr_;
        }
        bool operator!=(const Iterator& rhs) const{
            return ptr_ != rhs.ptr_;
        }
    private:
        T* ptr_;
    };

    List() : Base(nullptr, nullptr, 0){

    }
    List(const List&) = delete;
    List(List&& other)  noexcept : Base(other.head_, other.tail_, other.size_){
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
    }

    // must unlink all elements from the list
    ~List(){
        while (head_){
            auto next = head_->next_;
            head_->Unlink();
            head_ = next;
        }
    }

    List& operator=(const List&) = delete;
    List& operator=(List&& other) noexcept {
        size_ = other.size_;
        head_ = other.head_;
        tail_ = other.tail_;
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
        return *this;
    }

    [[nodiscard]] bool IsEmpty() const{
        return size_ == 0;
    }
    // that method is allowed to be O(n)
    [[nodiscard]] size_t Size() const{
        return size_;
    }

    // note that IntrusiveList doesn't own elements,
    // and never copies or moves T
    void PushBack(T* elem){
        if (tail_){
            ++size_;
            tail_->next_ = elem;
            elem->prev_ = tail_;
            tail_ = elem;
        }
        else{
            head_ = elem;
            tail_ = elem;
            size_ = 1;
        }
        elem->list = this;
        elem->isLinked_ = true;
    }
    void PushFront(T* elem){
        if (head_){
            ++size_;
            elem->next_ = head_;
            head_->prev_ = elem;
            head_ = elem;
        }
        else{
            head_ = elem;
            tail_ = elem;
            size_ = 1;
        }
        elem->list = this;
        elem->isLinked_ = true;
    }

    T& Front(){
        return *static_cast<T*>(static_cast<void*>(head_));
    }
    const T& Front() const{
        return *static_cast<T*>(static_cast<void*>(head_));
    }

    T& Back(){
        return *static_cast<T*>(static_cast<void*>(tail_));
    }
    const T& Back() const{
        return *static_cast<T*>(static_cast<void*>(tail_));
    }

    void PopBack(){
        tail_->isLinked_ = false;
        if (tail_ == head_){
            tail_ = nullptr;
            head_ = nullptr;
            size_ = 0;
        }
        else{
            auto newTail = tail_->prev_;
            tail_->prev_ = nullptr;
            --size_;
            tail_ = newTail;
        }
    }
    void PopFront(){
        head_->isLinked_ = false;
        if (tail_ == head_){
            tail_ = nullptr;
            head_ = nullptr;
            size_ = 0;
        }
        else{
            auto newHead = head_->next_;
            head_->next_ = nullptr;
            --size_;
            head_ = newHead;
        }
    }

    Iterator Begin(){
        return Iterator(static_cast<T*>(static_cast<void*>(head_)));
    }
    Iterator End(){
        return Iterator(nullptr);
    }

    // complexity of this function must be O(1)
    Iterator IteratorTo(T* element){
        return Iterator(static_cast<T*>(static_cast<void*>(element)));
    }
};

template <typename T>
typename List<T>::Iterator begin(List<T>& list) {  // NOLINT
    return list.Begin();
}

template <typename T>
typename List<T>::Iterator end(List<T>& list) {  // NOLINT
    return list.End();
}
