#include "utils/resources_manager.h"
#include <doctest/doctest.h>
#include <functional>

namespace utils::test {

TEST_SUITE("resources_manager")
{
    TEST_CASE("simple")
    {
        ResourcesManager manager;
        CHECK_THROWS(manager.get<int>("a"));
        ResourceHandle<int> handle = manager.get<int>("a", [] { return std::make_shared<int>(4); });
        CHECK(*handle == 4);
        manager.get_to("a", handle, [] { return std::make_shared<int>(6); });
        CHECK(*handle == 4);
        {
            ResourceHandle<int> handle_a = handle;
            manager.get_to("b", handle, [] { return std::make_shared<int>(6); });
            CHECK(*handle == 6);
            CHECK(*handle_a == 4);
            manager.get_to("a", handle);
            CHECK(*handle == 4);
        }
        CHECK_THROWS(manager.get<int>("b"));
    }
}

}  // namespace utils::test
