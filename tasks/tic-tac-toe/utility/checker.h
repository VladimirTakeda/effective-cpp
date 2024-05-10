#pragma once

#include "../strategy.h"


class Checker {
public:
    Checker(Strategy *first, Strategy * second) : m_first(first), m_second(second){
        m_first->Init(0);
        m_second->Init(1);
    }
    bool IsWin(char winner){
        if (m_matrix[0][0] == m_matrix[0][1] && m_matrix[0][1] == m_matrix[0][2] && m_matrix[0][0] == winner){
            return true;
        }
        if (m_matrix[1][0] == m_matrix[1][1] && m_matrix[1][1] == m_matrix[1][2] && m_matrix[1][0] == winner){
            return true;
        }
        if (m_matrix[2][0] == m_matrix[2][1] && m_matrix[2][1] == m_matrix[2][2] && m_matrix[2][0] == winner){
            return true;
        }
        if (m_matrix[0][0] == m_matrix[1][0] && m_matrix[1][0] == m_matrix[2][0] && m_matrix[0][0] == winner){
            return true;
        }
        if (m_matrix[0][1] == m_matrix[1][1] && m_matrix[1][1] == m_matrix[2][1] && m_matrix[0][1] == winner){
            return true;
        }
        if (m_matrix[0][2] == m_matrix[1][2] && m_matrix[1][2] == m_matrix[2][2] && m_matrix[0][2] == winner){
            return true;
        }
        if (m_matrix[0][0] == m_matrix[1][1] && m_matrix[1][1] == m_matrix[2][2] && m_matrix[0][0] == winner){
            return true;
        }
        if (m_matrix[0][2] == m_matrix[1][1] && m_matrix[1][1] == m_matrix[2][0] && m_matrix[0][2] == winner){
            return true;
        }
        return false;
    }
    int CheckWinner(){
        auto isValid = [this] (const std::pair<int, int>& elem){
            if (elem.first < 0 || elem.first > 2)
                return false;
            if (elem.second < 0 || elem.second > 2)
                return false;
            if (this->m_matrix[elem.first][elem.second] == 'X' || this->m_matrix[elem.first][elem.second] == 'O')
                return false;
            return true;
        };
        while (true){
            auto pos = m_first->MakeMove();
            if (!isValid(pos))
                return -1;
            m_matrix[pos.first][pos.second] = 'X';
            m_second->OpponentMove(pos);
            if (IsWin('X'))
                return 0;
            ++m_Xcount;
            if (m_Xcount == 5)
                return 2;

            pos = m_second->MakeMove();
            if (!isValid(pos))
                return -1;
            m_matrix[pos.first][pos.second] = 'O';
            m_first->OpponentMove(pos);
            if (IsWin('O'))
                return 1;
        }
    }

    int m_Xcount = 0;
    Strategy *m_first = nullptr;
    Strategy *m_second = nullptr;
    char m_matrix[3][3]{};
};
