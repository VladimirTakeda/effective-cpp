#pragma once

#include <vector>
#include <string>

class COWVector {
public:
    COWVector() : refcount(new size_t(1)), m_thisData(new std::vector<std::string>){

    }
    COWVector(const COWVector& other) : refcount(other.refcount){
        m_thisData = other.m_thisData;
        ++(*refcount);
    }
    COWVector& operator=(const COWVector& other){
        if (this == &other)
            return *this;
        DecrementThisCount();
        m_thisData = other.m_thisData;
        refcount = other.refcount;
        ++(*refcount);
        return *this;
    }
    COWVector(COWVector&& other) noexcept {
        DecrementThisCount();
        m_thisData = other.m_thisData;
        refcount = other.refcount;
        other.refcount = nullptr;
    }
    COWVector& operator=(COWVector&& other) noexcept {
        DecrementThisCount();
        m_thisData = other.m_thisData;
        refcount = other.refcount;
        other.refcount = nullptr;
        return *this;
    }

    ~COWVector() {
        DecrementThisCount();
    };

    [[nodiscard]] size_t Size() const{
        return m_thisData->size();
    }

    void Resize(size_t size){
        ApplySharedData();
        m_thisData->resize(size);
    }

    const std::string& Get(size_t at){
        return m_thisData->operator[](at);
    }
    const std::string& Back(){
        return m_thisData->back();
    }

    void PushBack(const std::string& value){
        ApplySharedData();
        m_thisData->push_back(value);
    }

    void Set(size_t at, const std::string& value){
        ApplySharedData();
        m_thisData->operator[](at) = value;
    }

    [[nodiscard]] size_t RefCount() const{
        return *refcount;
    }

private:
    void DecrementThisCount(){
        --(*refcount);
        if (*refcount == 0) {
            delete refcount;
            delete m_thisData;
        }
    }

    void ApplySharedData(){
        if (*refcount > 1){
            --(*refcount);
            auto* newData = new std::vector<std::string>();
            *newData = *m_thisData;
            m_thisData = newData;
            refcount = new size_t(1);
        }
    }

private:
    size_t *refcount;
    std::vector<std::string> *m_thisData;
};
