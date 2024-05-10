#include <catch.hpp>
#include <strict_iterator.h>
#include <local_max.h>

#include <vector>
#include <random>

struct JustInt {
    JustInt() {
    }
    JustInt(int x) : x(x) {
    }
    int x;

    bool operator<(const JustInt& r) const {
        return x < r.x;
    }
};

template <class T>
void CheckLocalMax(const std::vector<T>& data, size_t ok) {
    auto first = MakeStrict(data.cbegin(), data.cbegin(), data.cend());
    auto last = MakeStrict(data.cbegin(), data.cend(), data.cend());

    auto expected = MakeStrict(data.cbegin(), data.cbegin() + ok, data.cend());
    auto it = LocalMax(first, last);

    REQUIRE(it == expected);
}

TEST_CASE("simple") {
    std::vector<int> data{1, 2, 3, 4, 4, 3, 5, 4};
    CheckLocalMax(data, 6);
}

TEST_CASE("empty") {
    std::vector<int> data;
    CheckLocalMax(data, 0);
}

TEST_CASE("small") {
    {
        std::vector<int> data{1};
        CheckLocalMax(data, 0);
    }
    {
        std::vector<int> data{1, 1};
        CheckLocalMax(data, 2);
    }
    {
        std::vector<int> data{1, 2};
        CheckLocalMax(data, 1);
    }
    {
        std::vector<int> data{2, 1};
        CheckLocalMax(data, 0);
    }
}

TEST_CASE("tricky") {
    {
        std::vector<JustInt> data{JustInt(1), JustInt(2), JustInt(1)};
        CheckLocalMax(data, 1);
    }
    {
        std::vector<JustInt> data{JustInt(2), JustInt(1), JustInt(1)};
        CheckLocalMax(data, 0);
    }
    {
        std::vector<JustInt> data{JustInt(0), JustInt(1), JustInt(2)};
        CheckLocalMax(data, 2);
    }
    {
        std::vector<JustInt> data{JustInt(1), JustInt(1), JustInt(1)};
        CheckLocalMax(data, 3);
    }
}

TEST_CASE("big") {
    std::mt19937 gen(73475);
    const int count = 1e5;
    std::vector<int> data(count);
    std::uniform_int_distribution<int> dist(0, count - 1);
    int ind = dist(gen);
    data[ind] = 1;
    auto first = MakeStrict(data.cbegin(), data.cbegin(), data.cend());
    auto last = MakeStrict(data.cbegin(), data.cend(), data.cend());

    auto expected = MakeStrict(data.cbegin(), data.cbegin() + ind, data.cend());
    auto it = LocalMax(first, last);

    REQUIRE(it == expected);
}
