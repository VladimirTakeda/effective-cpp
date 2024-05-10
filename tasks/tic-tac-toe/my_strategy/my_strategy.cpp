#include "my_strategy.h"

extern "C" {
Strategy* CreateStrategy() {
    return new MyStrategy();
}
}

void MyStrategy::Init(int player_num){
    m_mySymbol = 'X';
    m_opponentSymbol = 'O';
    if (player_num == 1)
        std::swap(m_mySymbol, m_opponentSymbol);
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            m_matrix[i][j] = 0;
        }
    }
}

int MyStrategy::CountWays(char toCheck, char opposite){
    int count = 0;
    for (int i = 0; i < 3; ++i){
        int toCheckCount = 0;
        int oppositeCount = 0;
        for (int j = 0; j < 3; ++j){
            if (m_matrix[i][j] == toCheck){
                ++toCheckCount;
            }
            if (m_matrix[i][j] == opposite){
                ++oppositeCount;
            }
            if (toCheckCount == 2 && oppositeCount == 0){
                ++count;
            }
        }
    }
    for (int i = 0; i < 3; ++i){
        int toCheckCount = 0;
        int oppositeCount = 0;
        for (int j = 0; j < 3; ++j){
            if (m_matrix[i][j] == toCheck){
                ++toCheckCount;
            }
            if (m_matrix[i][j] == opposite){
                ++oppositeCount;
            }
            if (toCheckCount == 2 && oppositeCount == 0){
                ++count;
            }
        }
    }

    int toCheckCount = 0;
    int oppositeCount = 0;
    {
        for (int i = 0; i < 3; ++i) {
            if (m_matrix[i][i] == toCheck) {
                ++toCheckCount;
            }
            if (m_matrix[i][i] == opposite) {
                ++oppositeCount;
            }
            if (toCheckCount == 2 && oppositeCount == 0) {
                ++count;
            }
        }
    }

    toCheckCount = 0;
    oppositeCount = 0;

    {
        for (int i = 0; i < 3; ++i) {
            if (m_matrix[i][2 - i] == toCheck) {
                ++toCheckCount;
            }
            if (m_matrix[i][2 - i] == opposite) {
                ++oppositeCount;
            }
            if (toCheckCount == 2 && oppositeCount == 0) {
                ++count;
            }
        }
    }

    return count;
}

std::optional<std::pair<int, int>> MyStrategy::TwoWinWays(char toCheck, char opposite){
    std::optional<std::pair<int, int>> answer;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; ++j){
            if (m_matrix[i][j] != toCheck && m_matrix[i][j] != opposite){
                m_matrix[i][j] = toCheck;
                if (CountWays(toCheck, opposite) > 1)
                    answer = std::pair(i, j);
                m_matrix[i][j] = 0;
            }
            if (answer)
                break;
        }
        if (answer)
            break;
    }
    return answer;
}

std::optional<std::pair<int, int>> MyStrategy::CheckRows(char toCheck, char opposite){
    for (int i = 0; i < 3; ++i){
        int toCheckCount = 0;
        int oppositeCount = 0;
        for (int j = 0; j < 3; ++j){
            if (m_matrix[i][j] == toCheck){
                ++toCheckCount;
            }
            if (m_matrix[i][j] == opposite){
                ++oppositeCount;
            }
            if (toCheckCount == 2 && oppositeCount == 0){
                for (int k = 0; k < 3; k++){
                    if (m_matrix[i][k] != toCheck)
                        return std::make_pair(i, k);
                }
            }
        }
    }
    return std::nullopt;
}

std::optional<std::pair<int, int>> MyStrategy::CheckCols(char toCheck, char opposite){
    for (int i = 0; i < 3; ++i){
        int toCheckCount = 0;
        int oppositeCount = 0;
        for (int j = 0; j < 3; ++j){
            if (m_matrix[j][i] == toCheck){
                ++toCheckCount;
            }
            if (m_matrix[j][i] == opposite){
                ++oppositeCount;
            }
            if (toCheckCount == 2 && oppositeCount == 0){
                for (int k = 0; k < 3; k++){
                    if (m_matrix[k][i] != toCheck)
                        return std::make_pair(k, i);
                }
            }
        }
    }
    return std::nullopt;
}

std::optional<std::pair<int, int>> MyStrategy::CheckDiags(char toCheck, char opposite){
    int toCheckCount = 0;
    int oppositeCount = 0;
    {
        for (int i = 0; i < 3; ++i) {
            if (m_matrix[i][i] == toCheck) {
                ++toCheckCount;
            }
            if (m_matrix[i][i] == opposite) {
                ++oppositeCount;
            }
        }
    }

    if (toCheckCount == 2 && oppositeCount == 0) {
        for (int k = 0; k < 3; k++) {
            if (m_matrix[k][k] != toCheck)
                return std::make_pair(k, k);
        }
    }
    
    toCheckCount = 0;
    oppositeCount = 0;

    {
        for (int i = 0; i < 3; ++i) {
            if (m_matrix[i][2 - i] == toCheck) {
                ++toCheckCount;
            }
            if (m_matrix[i][2 - i] == opposite) {
                ++oppositeCount;
            }
        }
    }

    if (toCheckCount == 2 && oppositeCount == 0) {
        for (int k = 0; k < 3; k++) {
            if (m_matrix[k][2 - k] != toCheck)
                return std::make_pair(k, 2 - k);
        }
    }

    return std::nullopt;
}

// алгоритм на X (цель победить) :
// 1) центр
// 2) угол на линии или столбце которого есть 0
// 3) чек могу ли я выиграть, если нет, чек может ли выиграть соперник, если нет, то туда где даст две выиграшные позиции, если нет то в любое место

// алгоритм на 0 (цель не дать выиграть опоненту) :
// 1) если свободен центр - то центр, если нет, то любой угол
// 2) если опонент выигравает, не даём выиграть, если нет, то пытаемся выиграть сами, только не по диагонали
// 3) чек могу ли я выиграть, если нет, чек может ли выиграть соперник, если нет, то туда где даст две выиграшные позиции, если нет то в любое место
std::pair<int, int> MyStrategy::MakeMove(){
    if (m_mySymbol == 'X'){
        std::pair<int, int> move = MakeXMove();
        m_matrix[move.first][move.second] = m_mySymbol;
        return move;
    }
    std::pair<int, int> move = MakeOMove();
    m_matrix[move.first][move.second] = m_mySymbol;
    return move;
}

std::pair<int, int> MyStrategy::MakeXMove(){
    ++currMove;
    if (currMove == 1){
        return {1,1};
    }
    if (currMove == 2){
        if (m_matrix[0][0] != m_opponentSymbol && m_matrix[2][2] != m_opponentSymbol)
            return {0, 0};
        if (m_matrix[0][2] != m_opponentSymbol && m_matrix[2][0] != m_opponentSymbol)
            return {0, 2};
    }
    if (currMove > 2){
        std::optional<std::pair<int, int>> win;
        win = CheckCols(m_mySymbol, m_opponentSymbol);
        if (win)
            return win.value();
        win = CheckRows(m_mySymbol, m_opponentSymbol);
        if (win)
            return win.value();
        win = CheckDiags(m_mySymbol, m_opponentSymbol);
        if (win)
            return win.value();

        std::optional<std::pair<int, int>> draw;
        draw = CheckCols(m_opponentSymbol, m_mySymbol);
        if (draw)
            return draw.value();
        draw = CheckRows(m_opponentSymbol, m_mySymbol);
        if (draw)
            return draw.value();
        draw = CheckDiags(m_opponentSymbol, m_mySymbol);
        if (draw)
            return draw.value();

        auto twoWinWays = TwoWinWays(m_mySymbol, m_opponentSymbol);
        if (twoWinWays)
            return twoWinWays.value();

        for (int i = 0; i < 3; ++i){
            for (int j = 0; j < 3; j++){
                if (m_matrix[i][j] != m_mySymbol && m_matrix[i][j] != m_opponentSymbol)
                    return std::pair{i, j};
            }
        }
    }
}

std::pair<int, int> MyStrategy::MakeOMove(){
    ++currMove;
    if (currMove == 1){
        if (m_matrix[1][1] != m_opponentSymbol) {
            return {1, 1};
        }
        return {0, 0};
    }
    if (currMove == 2) {
        std::optional<std::pair<int, int>> draw;
        draw = CheckCols(m_opponentSymbol, m_mySymbol);
        if (draw)
            return draw.value();
        draw = CheckRows(m_opponentSymbol, m_mySymbol);
        if (draw)
            return draw.value();
        draw = CheckDiags(m_opponentSymbol, m_mySymbol);
        if (draw)
            return draw.value();

        if (m_matrix[0][1] == m_opponentSymbol) {
            return {1, 0};
        } else {
            return {0, 1};
        }
    }
    if (currMove > 2){
        std::optional<std::pair<int, int>> win;
        win = CheckCols(m_mySymbol, m_opponentSymbol);
        if (win)
            return win.value();
        win = CheckRows(m_mySymbol, m_opponentSymbol);
        if (win)
            return win.value();
        win = CheckDiags(m_mySymbol, m_opponentSymbol);
        if (win)
            return win.value();

        std::optional<std::pair<int, int>> draw;
        draw = CheckCols(m_opponentSymbol, m_mySymbol);
        if (draw)
            return draw.value();
        draw = CheckRows(m_opponentSymbol, m_mySymbol);
        if (draw)
            return draw.value();
        draw = CheckDiags(m_opponentSymbol, m_mySymbol);
        if (draw)
            return draw.value();

        auto twoWinWays = TwoWinWays(m_mySymbol, m_opponentSymbol);
        if (twoWinWays)
            return twoWinWays.value();

        for (int i = 0; i < 3; ++i){
            for (int j = 0; j < 3; j++){
                if (m_matrix[i][j] != m_mySymbol && m_matrix[i][j] != m_opponentSymbol)
                    return std::pair{i, j};
            }
        }
    }
}

void MyStrategy::OpponentMove(const std::pair<int, int>& move) {
    m_matrix[move.first][move.second] = m_opponentSymbol;
}
