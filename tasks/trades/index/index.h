#pragma once

#include <vector>
#include <numbers>

#include "../trade.h"

class TradeIndex{
public:
    TradeIndex(const std::vector<Trade> &input);
    int64_t TotalVolume(int64_t from, int64_t to);

    std::vector<std::pair<int64_t, int64_t>> m_data;
};