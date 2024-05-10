#pragma once

#include <initializer_list>
#include <algorithm>
#include <deque>
#include <iostream>

using namespace std;

class Deque {
public:
    Deque() : m_elemCount(0), m_blockSize(512 / sizeof(int)), m_frontIndex(0), m_backIndex(0) {
        blocks.emplace_back(m_blockSize);
    }
    Deque(std::initializer_list<int> input) : Deque(){
        for (int elem : input){
            PushBack(elem);
        }
    }

    [[nodiscard]] bool Empty() const {
        return m_elemCount == 0;
    }

    void PushBack(int elem){
        if (blocks.empty() || m_backIndex == m_blockSize) {
            blocks.emplace_back(m_blockSize);
            m_backIndex = 0;
        }
        ++m_elemCount;
        blocks.back()[m_backIndex++] = elem;
    }
    void PopBack(){
        if (Empty())
            throw std::out_of_range("deque is empty");

        if (m_backIndex == 0) {
            // если было два push_front и 1 pop_back то удалится единственный блок
            blocks.pop_back();
            m_backIndex = m_blockSize;
        }

        --m_backIndex;
        --m_elemCount;
        if (m_elemCount == 0)
            Clear();
    }

    void PushFront(int elem){
        if (blocks.empty() || m_frontIndex == 0) {
            blocks.insert(blocks.begin(), std::vector<int>(m_blockSize));
            m_frontIndex = m_blockSize;
        }
        ++m_elemCount;
        blocks.front()[--m_frontIndex] = elem;
    }

    void PopFront(){
        if (Empty())
            throw std::out_of_range("deque is empty");

        if (++m_frontIndex == m_blockSize) {
            blocks.erase(blocks.begin());
            m_frontIndex = 0;
        }
        --m_elemCount;
        if (m_elemCount == 0)
            Clear();
    }

    int& operator[](size_t index){
        if (index >= Size())
            throw std::out_of_range("index out of range");

        if (index < m_blockSize - m_frontIndex)
            return blocks.front()[m_frontIndex + index];
        else {
            size_t blockIndex = (index - (m_blockSize - m_frontIndex)) / m_blockSize + 1;
            size_t elementIndex = (index - (m_blockSize - m_frontIndex)) % m_blockSize;
            return blocks[blockIndex][elementIndex];
        }
    }

    const int& operator[](size_t index) const {
        if (index >= Size())
            throw std::out_of_range("index out of range");

        if (index < m_blockSize - m_frontIndex)
            return blocks.front()[m_frontIndex + index];
        else {
            size_t blockIndex = (index - (m_blockSize - m_frontIndex)) / m_blockSize + 1;
            size_t elementIndex = (index - (m_blockSize - m_frontIndex)) % m_blockSize;
            return blocks[blockIndex][elementIndex];
        }
    }

    [[nodiscard]] size_t Size() const{
        return m_elemCount;
    }

    void Clear(){
        blocks.clear();
        m_elemCount = 0;
        m_frontIndex = 0;
        m_backIndex = 0;
    }

    void Swap(Deque& other){
        using std::swap;
        swap(m_elemCount, other.m_elemCount);
        swap(blocks, other.blocks);
        swap(m_frontIndex, other.m_frontIndex);
        swap(m_backIndex, other.m_backIndex);
    }

private:
    std::vector<std::vector<int>> blocks;
    size_t m_elemCount;
    size_t m_blockSize;
    size_t m_frontIndex;
    size_t m_backIndex;
};
