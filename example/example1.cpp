#include <utest/utest.hpp>


TEST_CASE("test 1") {
    ASSERT(2 * 2 == 4);
}

TEST_CASE("test 2") {
    // false assertion with message
    ASSERT(0 > 1, "false assertion with message");
    ASSERT(1 + 1 == 2);
}


TEST_CASE("test 3") {
    // false assertion without message
    ASSERT(false);
    ASSERT(5 + 5 == 10);
}

DEFINE_TEST_RUNNER_HERE();