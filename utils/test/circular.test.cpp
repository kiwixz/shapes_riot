#include "utils/circular.h"

#include <doctest/doctest.h>

namespace utils::test {

TEST_SUITE("circular")
{
    TEST_CASE("simple")
    {
        Circular<int, 3> c;
        CHECK(c.empty());
        CHECK(c.size() == 0);
        CHECK(c.max_size() == 3);

        c.push_back(1);
        CHECK(!c.empty());
        CHECK(c.size() == 1);
        CHECK(c.front() == 1);
        CHECK(c.back() == 1);

        c.push_back(2);
        CHECK(c.size() == 2);
        CHECK(c.front() == 1);
        CHECK(c.back() == 2);

        c.push_front(0);
        CHECK(c.size() == 3);
        CHECK(c.front() == 0);
        CHECK(c.back() == 2);

        c.push_back(3);
        CHECK(c.size() == 3);
        CHECK(c.front() == 1);
        CHECK(c.back() == 3);

        c.push_back(4);
        CHECK(c.size() == 3);
        CHECK(c.front() == 2);
        CHECK(c.back() == 4);

        c.push_front(1);
        CHECK(c.size() == 3);
        CHECK(c.front() == 1);
        CHECK(c.back() == 3);
    }
}

}  // namespace utils::test
