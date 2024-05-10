#include "index.h"

    TradeIndex::TradeIndex(const std::vector<Trade> &input){
        m_data.resize(input.size() + 1);
        for (size_t i = 0; i < input.size(); ++i){
            m_data[i + 1] = {input[i].timestamp, input[i].volume * input[i].price + m_data[i].second};
        }
        std::sort(m_data.begin(), m_data.end());
    }

    int64_t TradeIndex::TotalVolume(int64_t from, int64_t to){
        auto start = std::upper_bound(m_data.begin(), m_data.end(), from - 1, [] (int64_t value, const std::pair<int64_t, int64_t> &pair){
            return value < pair.first;
        });
        auto end = std::upper_bound(m_data.begin(), m_data.end(), to - 1, [] (int64_t value, const std::pair<int64_t, int64_t> &pair){
            return value < pair.first ;
        });
        return std::prev(end)->second - std::prev(start)->second;
    }