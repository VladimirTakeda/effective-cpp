#pragma once

#include <vector>
#include <optional>

#include "../strategy.h"


class MyStrategy: public Strategy{
    void Init(int player_num) override;
    std::pair<int, int> MakeMove() override;
    std::pair<int, int> MakeXMove();
    std::pair<int, int> MakeOMove();
    int CountWays (char toCheck, char opposite);
    std::optional<std::pair<int, int>> TwoWinWays(char toCheck, char opposite);
    std::optional<std::pair<int, int>> CheckRows(char toCheck, char opposite);
    std::optional<std::pair<int, int>> CheckCols(char toCheck, char opposite);
    std::optional<std::pair<int, int>> CheckDiags(char toCheck, char opposite);
    void OpponentMove(const std::pair<int, int>& move) override;
    ~MyStrategy() override = default;

    int currMove = 0;
    char m_mySymbol;
    char m_opponentSymbol;
    char m_matrix[3][3];
};

extern "C" {
Strategy* CreateStrategy();
}
