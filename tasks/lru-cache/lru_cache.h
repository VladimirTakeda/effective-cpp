#pragma once

#include <optional>
#include <list>

template<class K, class V>
class LruCache {
public:
    explicit LruCache(size_t capacity) : m_capacity(capacity){}

    const V* Get(const K& key) {
        if (auto iter = m_cache.find(key); iter != m_cache.end()) {
            auto pair = *iter->second;
            m_data.erase(iter->second);
            m_data.emplace_back(pair);
            m_cache[key] = std::prev(m_data.end());
            return &m_data.back().second;
        }
        return nullptr;
    }
    void Set(const K& key, V value) {
        if (auto iter = m_cache.find(key); iter != m_cache.end()) {
            m_data.erase(iter->second);
            m_data.emplace_back(key, value);
            m_cache[key] = std::prev(m_data.end());
        }
        else{
            if (m_data.size() == m_capacity) {
                m_cache.erase(m_data.front().first);
                m_data.pop_front();
            }
            m_data.emplace_back(key, value);
            m_cache[key] = std::prev(m_data.end());
        }
    }
    std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> m_cache;
    std::list<std::pair<K, V>> m_data;
    size_t m_capacity;
};
