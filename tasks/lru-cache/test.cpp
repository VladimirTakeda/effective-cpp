#include <catch.hpp>
#include <lru_cache.h>
#include <random>
#include <string>
#include <algorithm>


TEST_CASE("set_get") {
    LruCache<std::string, std::string> cache(10);

    cache.Set("a", "1");
    cache.Set("b", "2");
    cache.Set("c", "3");

    REQUIRE(*cache.Get("a") == "1");
    REQUIRE(*cache.Get("b") == "2");
    REQUIRE(*cache.Get("c") == "3");

    REQUIRE(!cache.Get("d"));
    cache.Set("c", "4");
    REQUIRE(*cache.Get("c") == "4");
}

TEST_CASE("eviction") {
    LruCache<std::string, std::string> cache(2);

    cache.Set("a", "1");
    cache.Set("b", "2");
    cache.Set("c", "3");

    REQUIRE(!cache.Get("a"));
    REQUIRE(*cache.Get("b") == "2");
    REQUIRE(*cache.Get("c") == "3");

    cache.Set("b", "4");
    cache.Set("c", "5");
    cache.Set("b", "6");

    cache.Set("e", "7");
    REQUIRE(!cache.Get("c"));
    REQUIRE(*cache.Get("b") == "6");
    REQUIRE(*cache.Get("e") == "7");

    (void)cache.Get("b");
    cache.Set("f", "8");
    REQUIRE(!cache.Get("e"));
    REQUIRE(*cache.Get("b") == "6");
    REQUIRE(*cache.Get("f") == "8");
}

TEST_CASE("stress") {
    const int max_size = 1e5;
    std::mt19937 gen(2747);
    std::vector<int> data(max_size);
    for (int i = 0; i < max_size; ++i) {
        data[i] = i;
    }
    std::shuffle(data.begin(), data.end(), gen);

    LruCache<int, int> cache(max_size);
    for (auto x : data) {
        cache.Set(x, max_size - x);
    }

    for (int i = 0; i < max_size; ++i) {
        REQUIRE(*cache.Get(i) == max_size - i);
    }
    for (int i = 0; i < max_size; ++i) {
        cache.Set(-i, i);
    }
    for (int i = 1; i < max_size; ++i) {
        REQUIRE(!cache.Get(i));
        REQUIRE(*cache.Get(-i) == i);
    }
}
