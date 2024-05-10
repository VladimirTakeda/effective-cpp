#pragma once

#include <fstream>
#include <numbers>
#include <optional>
#include <vector>

#include "../trade.h"

class TradeReader {
public:
    explicit TradeReader(std::istream& in);
    std::optional<Trade> ReadNext();
    std::vector<Trade> ReadAll();

    std::istream& m_in;
};
