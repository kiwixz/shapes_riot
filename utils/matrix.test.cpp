#include "utils/matrix.h"
#include <doctest/doctest.h>

namespace utils::test {

TEST_SUITE("matrix")
{
    TEST_CASE("constexpr")
    {
        constexpr Matrix2i matrix;
        static_assert(matrix(0, 0) == 0);
    }
}

}  // namespace utils::test
