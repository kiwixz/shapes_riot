#include "utils/shared_handle.h"
#include <doctest/doctest.h>

namespace utils::test {
namespace {

template <typename T>
int& global()
{
    static int c;
    return c;
}

}  // namespace


TEST_SUITE("shared_handle")
{
    TEST_CASE("simple")
    {
        int& c = global<struct simple>();
        c = 100;

        static constexpr auto init = []() {
            global<struct simple>() *= 2;
        };
        static constexpr auto destroy = []() {
            global<struct simple>() /= 2;
        };
        using Handle = SharedHandle<init, destroy>;

        {
            Handle handle;
            CHECK(c == 200);
            Handle handle2 = handle;
            CHECK(c == 200);
            {
                Handle handle3;
                CHECK(c == 200);
            }
            CHECK(c == 200);
            Handle handle4 = std::move(handle);
            CHECK(c == 200);
        }
        CHECK(c == 100);
    }
}

}  // namespace utils::test
