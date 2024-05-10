#include <iostream>
#include <vector>
#include <cmath>

class CircularIterator {
public:
    using value_type = pair<double, double>;
    using reference = pair<double, double>&;
    using pointer = pair<double, double>*;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::input_iterator_tag;

    explicit CircularIterator(const std::vector<pair<double, double>>& prices, const std::vector<uint64_t>& mask, size_t currIndex) : m_prices(prices), m_mask(mask), m_currIndex(currIndex) {}

    CircularIterator& operator++() {
        do {
            ++m_currIndex;
            if (m_currIndex >= m_prices.size())
                m_currIndex = 0;
        } while (!isElementValid(m_currIndex));
        return *this;
    }

    const CircularIterator operator++(int) {
        CircularIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator!=(const CircularIterator& other) const {
        return m_currIndex != other.m_currIndex;
    }

    bool operator==(const CircularIterator& other) const {
        return m_currIndex == other.m_currIndex;
    }

    [[nodiscard]] bool isElementValid(size_t index) const {
        return (m_mask[index / 64] & (1ULL << (index % 64))) != 0;
    }

    const pair<double, double>& operator*() const {
        return m_prices[m_currIndex];
    }

    const pair<double, double>* operator->() const{
        return &m_prices[m_currIndex];
    }

    const std::vector<pair<double, double>>& m_prices;
    const std::vector<uint64_t>& m_mask;

    size_t m_currIndex;
};

class CircularOrderBook {
private:
    static constexpr int ARRAY_SIZE = 100000;

    std::vector<pair<double, double>> prices;
    std::vector<uint64_t> mask;

    uint64_t minPrice = 0;
    uint64_t maxPrice = 0;

    size_t minIndex = 0;

public:
    CircularOrderBook() : prices(ARRAY_SIZE), mask(ARRAY_SIZE / 64), minPrice(0.0), maxPrice(0.0) {}

    void insert(double price, double volume) {
        while (std::abs(price - static_cast<int>(price)) > 1e-9) {
            price *= 10;
        }

        if (price < minPrice || minPrice == 0)
            minPrice = price;
        if (price > maxPrice)
            maxPrice = price;

        int index = static_cast<int>(std::round(price - minPrice)) % ARRAY_SIZE;

        prices[index] = make_pair(price, volume);
        mask[index / 64] |= (1ULL << (index % 64));
    }

    double& operator[](double price) {
        while (std::abs(price - static_cast<int>(price)) > 1e-9) {
            price *= 10;
        }

        if (price < minPrice || minPrice == 0.0)
            minPrice = price;
        if (price > maxPrice)
            maxPrice = price;

        int index = static_cast<int>(price - minPrice) % ARRAY_SIZE;

        return prices[index].second;
    }

    void erase(double price) {
        while (std::abs(price - static_cast<int>(price)) > 1e-9) {
            price *= 10;
        }
        int index = static_cast<int>(std::round(price - minPrice)) % ARRAY_SIZE;
        mask[index / 64] &= ~(1ULL << (index % 64));
    }

    CircularIterator begin(){
        return CircularIterator(prices, mask, minIndex);
    }
};