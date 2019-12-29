#include "utils/math.h"

#include <doctest/doctest.h>

namespace utils::test {

TEST_SUITE("math")
{
    TEST_CASE("ceil")
    {
        static_assert(0 == ceil<int>(0.0));
        static_assert(4 == ceil<int>(4.f));
        static_assert(4 == ceil<char>(4));
        static_assert(5 == ceil<int>(4.5));
        static_assert(5 == ceil<int>(4.000'000'000'000'01));
        static_assert(5 == ceil<int>(4.999'999'999'999'99));

        static_assert(0 == ceil<int>(-0.0));
        static_assert(-4 == ceil<int>(-4.f));
        static_assert(-4 == ceil<char>(-4));
        static_assert(-4 == ceil<int>(-4.5));
        static_assert(-4 == ceil<int>(-4.000'000'000'000'01));
        static_assert(-4 == ceil<int>(-4.999'999'999'999'99));
    }

    TEST_CASE("ceil2")
    {
        CHECK(ceil2(0) == 1);
        CHECK(ceil2(1) == 1);
        CHECK(ceil2(2) == 2);
        CHECK(ceil2(3) == 4);
        CHECK(ceil2(4) == 4);
        CHECK(ceil2(5) == 8);
        CHECK(ceil2(-1) == -1);
        CHECK(ceil2(-2) == -2);
        CHECK(ceil2(-3) == -4);
        CHECK(ceil2(-4) == -4);
        CHECK(ceil2(-5) == -8);
    }

    TEST_CASE("ceil_div")
    {
        static_assert(ceil_div(0, 2) == 0);
        static_assert(ceil_div(1, 2) == 1);
        static_assert(ceil_div(2, 2) == 1);
        static_assert(ceil_div(3, 2) == 2);
        static_assert(ceil_div(4, 2) == 2);
        static_assert(ceil_div(5, 2) == 3);
    }
}

}  // namespace utils::test
