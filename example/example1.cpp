#include <utest/utest.hpp>


TEST_CASE("test 1") {
    ASSERT(2 * 2 == 4);
}

// false assertion?
TEST_CASE("test 2") {
    ASSERT(0 > 1);
    ASSERT(1 + 1 == 2);
}

TEST_CASE("test 3") {
    ASSERT(true);
    ASSERT(5 + 5 == 10);
}

DEFINE_TEST_RUNNER_HERE();