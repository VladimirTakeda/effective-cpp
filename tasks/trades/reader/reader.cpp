#include "reader.h"

    TradeReader::TradeReader(std::istream& in) : m_in(in){
    }

    std::optional<Trade> TradeReader::ReadNext(){
        Trade trade;
        m_in.read(reinterpret_cast<char*>(&trade), 28);
        if (m_in){
            return trade;
        }
        return std::nullopt;
    }

    std::vector<Trade> TradeReader::ReadAll() {
        std::vector<Trade> result;
        for (auto trade = ReadNext(); trade != std::nullopt; trade = ReadNext()) {
            result.push_back(trade.value());
        }
        return result;
    }
