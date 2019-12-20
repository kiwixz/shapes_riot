#include "utils/any.h"

#include <doctest/doctest.h>

namespace utils::test {

TEST_SUITE("any")
{
    TEST_CASE("simple")
    {
        Any a;
        CHECK(!a);
        a = 42;
        CHECK(a.as<int>() == 42);
        a = 42.0;
        CHECK(a.as<double>() == 42.0);

        Any b = a;
        a = 54;
        CHECK(a.as<int>() == 54);
        CHECK(b.as<double>() == 42.0);
    }
}

}  // namespace utils::test
