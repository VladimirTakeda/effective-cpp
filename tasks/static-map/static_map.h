#pragma once

#include <vector>

template<class K, class V>
class StaticMap {
public:
    explicit StaticMap(std::vector<std::pair<K, V>> items) {
        m_items = items;
        std::sort(m_items.begin(), m_items.end(), [](const std::pair<K, V>& left, const std::pair<K, V>& right)
        {
            return left.first < right.first;
        });
    }

    template<class Iterator>
    StaticMap(Iterator first, Iterator last) {
        m_items = std::vector<std::pair<K, V>>(first, last);
        std::sort(m_items.begin(), m_items.end());
    }

    const V* Find(const K& key) const {
        auto ans = std::lower_bound(m_items.begin(), m_items.end(), key,
                                    [](const std::pair<K, V>& info, K key)
                                    {
                                        return info.first < key;
                                    });
        if (ans == m_items.end() || key < ans->first)
            return nullptr;
        return &ans->second;
    }

    std::vector<std::pair<K, V>> m_items;
};
