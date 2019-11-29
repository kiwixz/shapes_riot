#include "utils/math.h"
#include <doctest/doctest.h>

namespace utils::test {

TEST_SUITE("math")
{
    TEST_CASE("div_ceil")
    {
        static_assert(div_ceil(0, 2) == 0);
        static_assert(div_ceil(1, 2) == 1);
        static_assert(div_ceil(2, 2) == 1);
        static_assert(div_ceil(3, 2) == 2);
        static_assert(div_ceil(4, 2) == 2);
        static_assert(div_ceil(5, 2) == 3);
    }

    TEST_CASE("ceil2")
    {
        CHECK(ceil2(0) == 1);
        CHECK(ceil2(1) == 1);
        CHECK(ceil2(2) == 2);
        CHECK(ceil2(3) == 4);
        CHECK(ceil2(4) == 4);
        CHECK(ceil2(5) == 8);
    }
}

}  // namespace utils::test
