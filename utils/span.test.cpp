#include "utils/span.h"
#include <doctest/doctest.h>
#include <array>
#include <vector>

namespace utils::test {

TEST_SUITE("span")
{
    TEST_CASE("simple")
    {
        std::vector<int> container = {1, 2, 3};
        Span<int> span = container;
        CHECK(*span.begin() == 1);
        CHECK(span.data() == container.data());
        CHECK(span[2] == 3);
        CHECK(span.size() == container.size());
        span[1] = 4;
        CHECK(container[1] == 4);
    }

    TEST_CASE("const")
    {
        std::vector<int> container = {1, 2, 3};
        Span<const int> span = container;
        CHECK(*span.begin() == 1);
        CHECK(span.data() == container.data());
        CHECK(span[2] == 3);
        CHECK(span.size() == container.size());
    }

    TEST_CASE("constexpr")
    {
        static constexpr std::array<const char, 6> container{"hello"};
        constexpr Span<const char> midspan = container;
        constexpr Span<const char> span{midspan.data(), midspan.size()};
        static_assert(*span.begin() == 'h');
        static_assert(span.data() == container.data());
        static_assert(span[2] == 'l');
        static_assert(span.size() == container.size());
    }
}

}  // namespace utils::test
