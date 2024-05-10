#include <vector>
#include <list>
#include <sstream>
#include <string>
#include <forward_list>

#include <catch.hpp>
#include <advance.h>
#include <clear.h>

TEST_CASE("forward", "[advance]") {
    std::forward_list<int> l{1, 2, 3, 4, 5};
    auto it = l.begin();
    Advance(it, 2);
    REQUIRE(3 == *it);
    Advance(it, 0);
    REQUIRE(3 == *it);
}

TEST_CASE("bidirectional", "[advance]") {
    std::list<std::string> l{"ab", "ba", "aba", "caba"};
    auto first = l.begin();
    Advance(first, 3);
    REQUIRE("caba" == *first);
    auto second = l.end();
    Advance(second, -3);
    REQUIRE("ba" == *second);
    Advance(second, 0);
    REQUIRE("ba" == *second);
}

TEST_CASE("randomAccess", "[advance]") {
    std::vector<int> l{1, 3, 5, 7, 9};
    auto first = l.begin();
    Advance(first, 1);
    REQUIRE(3 == *first);
    auto second = l.end();
    Advance(second, -4);
    REQUIRE(3 == *second);
}

TEST_CASE("randomAccessRaw", "[advance]") {
    int l[] = {-1, 1, 0, 4, 8};
    int *it = l;
    Advance(it, 2);
    REQUIRE(0 == *it);
    Advance(it, -1);
    REQUIRE(1 == *it);
}

class CheckIterator: public std::iterator<std::random_access_iterator_tag, int> {
public:
    CheckIterator& operator +=(ssize_t n) {
        ++was_called_;
        value_ += n;
        return *this;
    }

    CheckIterator& operator -=(ssize_t n) {
        ++was_called_;
        value_ -= n;
        return *this;
    }

    int CalledCount() const {
        return was_called_;
    }

    ssize_t GetValue() const {
        return value_;
    }
private:
    int was_called_ = 0;
    ssize_t value_ = 0;
};

TEST_CASE("RandomAccessFast", "[advance]") {
    for (int value : {-100, 100}) {
        CheckIterator iterator;
        Advance(iterator, value);
        REQUIRE(value == iterator.GetValue());
        REQUIRE(1 == iterator.CalledCount());
    }

    const int n = 100000;
    std::vector<int> v(n);
    auto begin = std::clock();
    for (int i = 0; i < n; ++i) {
        v[i] = i;
        const int *start = v.data();
        Advance(start, i);
        REQUIRE(i == *start);
    }
    auto end = std::clock();
    std::clock_t diff = end - begin;
    double passed = static_cast<double> (diff) / CLOCKS_PER_SEC;
    REQUIRE(passed <= 0.3);
}

TEST_CASE("Input", "[advance]") {
    std::stringstream stream("1 2 3 4 5");
    std::istream_iterator<int> it(stream);
    Advance(it, 3);
    REQUIRE(4 == *it);
}

TEST_CASE("Output", "[advance]") {
    std::vector<int> dummy;
    auto it = std::back_inserter(dummy);
    Advance(it, 1);
}


struct X {
    int x;
    explicit X(int x): x(x) {}

    void Clear() {
        x = 0;
    }
};

TEST_CASE("rawPointer", "[clear]") {
    X *data = new X(5);
    Clear(data);
    REQUIRE(data == nullptr);
}

TEST_CASE("smartPointer", "[clear]") {
    {
        auto unique = std::make_unique<X>(7);
        Clear(unique);
        REQUIRE(unique.get() == nullptr);
    }
    {
        auto s1 = std::make_shared<X>(13);
        auto s2 = s1;
        Clear(s1);
        REQUIRE(s1.get() == nullptr);
        REQUIRE(s2->x == 13);
    }
}

TEST_CASE("clear", "[clear]") {
    X x{8};
    Clear(x);
    REQUIRE(x.x == 0);
}

struct Y {
    int y;
    explicit Y(int y): y(y) {}
};

TEST_CASE("empty", "[clear]") {
    Y y{9};
    Clear(y);
    REQUIRE(y.y == 9);
}
