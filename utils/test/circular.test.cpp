#include "utils/circular.h"

#include <doctest/doctest.h>

namespace utils::test {

TEST_SUITE("circular")
{
    TEST_CASE("simple")
    {
        Circular<int, 3> c;
        CHECK(c.max_size() == 3);

        auto eq = [& cont = c](const std::initializer_list<int>& list) {
            const Circular<int, 3>& c = cont;

            if (list.size() == 0) {
                CHECK(c.empty());
                CHECK(c.size() == 0);
                return;
            }

            CHECK(!c.empty());
            CHECK(c.size() == list.size());
            CHECK(c.front() == *list.begin());
            CHECK(c.back() == *(list.end() - 1));

            auto it = list.begin();
            for (int i : c) {
                CHECK(i == *it);
                ++it;
            }
        };

        eq({});
        c.push_back(1);
        eq({1});
        c.push_back(2);
        eq({1, 2});
        c.push_front(0);
        eq({0, 1, 2});
        c.push_back(3);
        eq({1, 2, 3});
        c.push_back(4);
        eq({2, 3, 4});
        c.push_front(1);
        eq({1, 2, 3});

        c.pop_front();
        eq({2, 3});
        c.push_front(1);
        eq({1, 2, 3});
        c.pop_front();
        eq({2, 3});
        c.push_back(4);
        eq({2, 3, 4});

        c.pop_back();
        eq({2, 3});
        c.push_back(4);
        eq({2, 3, 4});
        c.pop_back();
        eq({2, 3});
        c.push_front(1);
        eq({1, 2, 3});

        c.push_back(4);
        eq({2, 3, 4});
        c.push_back(5);
        eq({3, 4, 5});
        c.push_back(6);
        eq({4, 5, 6});
        c.push_back(7);
        eq({5, 6, 7});
        c.push_back(8);
        eq({6, 7, 8});

        c.pop_front();
        eq({7, 8});
        c.pop_front();
        eq({8});
        c.pop_front();
        eq({});
    }
}

}  // namespace utils::test
