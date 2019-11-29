#include "gfx/transformation.h"
#include "utils/matrix.h"
#include "utils/vec.h"
#include <doctest/doctest.h>

namespace gfx::test {

TEST_SUITE("transformation")
{
    TEST_CASE("order")
    {
        utils::Matrix4d matrix = transformation<double>({1.0, 2.0, 3.0}, {2.0, 2.0, 2.0}, {});
        utils::Vec4d origin{4.0, 5.0, 6.0};
        utils::Vec4d result = matrix * origin;
        CHECK(result == utils::Vec4d{4.0 * 2.0 + 1.0, 5.0 * 2.0 + 2.0, 6.0 * 2.0 + 3.0});
    }
}

}  // namespace gfx::test
