#pragma once

#include <optional>
#include <utility>
#include <vector>
#include <cstdlib>
#include <iostream>

size_t nearestPowerOfTwo(size_t num) {
    if (num & (num - 1)) {
        size_t result = 1;
        while (result < num) {
            result <<= 1;
        }
        return result;
    } else {
        return num;
    }
}

template<class K, class V, class H = std::hash<K>>
class HashMap {
public:
    explicit HashMap(H hasher = H()) : m_hasher(hasher), m_capacity(8), m_size(0){
        m_data.resize(m_capacity);
    }

    explicit HashMap(size_t elements_count, H hasher = H()) : HashMap(hasher){
        m_capacity = nearestPowerOfTwo(elements_count);
        m_data.resize(m_capacity);
    }

    template<class Iterator>
    HashMap(Iterator first, Iterator last, H hasher = H()) : HashMap(hasher){
        for (auto it = first; it != last; it = std::next(it)){
            insert(*it);
        }
    }

    HashMap(std::initializer_list<std::pair<K, V>> l, H hasher = H()) : HashMap(hasher){
        for (auto &elem : l){
            insert(elem);
        }
    }

    [[nodiscard]] size_t size() const{
        return m_size;
    }

    [[nodiscard]] bool empty() const{
        return m_size == 0;
    }

    void insert(const K& key, V value){
        insert(std::make_pair(key, value));
    }


    void insert(const std::pair<K, V>& elem){
        size_t idx = m_hasher(elem.first);
        while (m_data[(idx + m_capacity) % m_capacity].first && m_data[(idx + m_capacity) % m_capacity].second.first != elem.first)
        {
            ++idx;
        }
        if (m_data[(idx + m_capacity) % m_capacity].first && m_data[(idx + m_capacity) % m_capacity].second.first == elem.first){
            m_data[(idx + m_capacity) % m_capacity] = {true, elem};
        }
        else{
            m_data[(idx + m_capacity) % m_capacity] = {true, elem};
            ++m_size;
        }
        if (m_size == m_capacity / 2) {
            rehash();
        }
    }

    // returns true if key exists
    bool erase(const K& key) {
        size_t idx = m_hasher(key) % m_capacity;
        while (m_data[idx].first && m_data[idx].second.first != key) {
            ++idx;
        }
        if (idx == m_capacity || !m_data[idx].first) {
            return false; // no such key
        }

        m_data[idx].first = false;

        auto currentFree = idx;
        idx = (idx + 1) % m_capacity;

        while (m_data[idx].first){
            int originalIndex = m_hasher(m_data[idx].second.first) % m_capacity;
            if (std::abs(int(idx - originalIndex)) > std::abs(int(currentFree - originalIndex))) {
                swap(m_data[currentFree], m_data[idx]);
                currentFree = idx;
            }
            idx = (idx + 1) % m_capacity;
        }

        --m_size;
        return true;
    }

    void clear(){
        m_data.clear();
        m_data.resize(8);
        m_capacity = 0;
        m_size = 0;
    }

    void rehash(){
        auto curr = m_data;
        m_data.clear();
        m_capacity *= 2;
        m_data.resize(m_capacity);
        m_size = 0;
        for (auto &&elem : curr){
            if (elem.first)
                insert(elem.second);
        }
    }

    V& operator[](const K& key){
        size_t idx = m_hasher(key) % m_capacity;
        while (m_data[idx].first && m_data[idx].second.first != key){
            ++idx;
        }
        if (!m_data[idx].first) {
            m_data[idx].first = true;
            m_data[idx].second.first = key;
            ++m_size;
        }
        if (m_size == m_capacity / 2) {
            rehash();
        }
        return m_data[idx].second.second;
    }

    class iterator{
    public:
        explicit iterator(std::vector<std::pair<bool, std::pair<K,V>>>::const_iterator curr, std::vector<std::pair<bool, std::pair<K,V>>>::const_iterator end) : m_end(end), m_curr(curr){

        }
        const std::pair<K,V>* operator->(){
            return &m_curr->second;
        }
        bool operator == (const iterator& right) const{
            return m_curr == right.m_curr;
        }
        iterator& operator++(){
            while(++m_curr != m_end && m_curr->first == K());
            return *this;
        }
        iterator operator++(int){
            auto ans = *this;
            while(++m_curr != m_end && m_curr->first == K());
            return ans;
        }
        const std::pair<K,V>& operator*(){
            return m_curr->second;
        }
        std::vector<std::pair<bool, std::pair<K,V>>>::const_iterator m_end;
        std::vector<std::pair<bool, std::pair<K,V>>>::const_iterator m_curr;
    };

    iterator find(const K& key) const{
        size_t idx = m_hasher(key) % m_capacity;
        while (idx < m_capacity && m_data[idx].second.first != key)
        {
            ++idx;
        }
        return iterator(m_data.begin() + idx, m_data.end());
    }

    iterator begin() const{
        auto m_curr = m_data.begin();
        while(m_curr != m_data.end() && !m_curr->first){
            ++m_curr;
        }
        return iterator(m_curr, m_data.end());
    }

    iterator end() const{
        return iterator(m_data.end(), m_data.end());
    }

    // return a copy of hasher object
    H hash_function() const{
        return m_hasher;
    }

    // for testing purposes
    // returns the bucket this key resides on
    // or -1 if table doesn't contain key
    ssize_t bucket(const K& key) const{
        size_t idx = m_hasher(key) % m_capacity;
        while (idx < m_capacity && m_data[idx].second.first != key)
        {
            ++idx;
        }
        return idx;
    }

private:
    H m_hasher;
    std::vector<std::pair<bool, std::pair<K,V>>> m_data;
    size_t m_capacity;
    size_t m_size;
};
