#include <utest/utest.hpp>
#include <string>

TEST_CASE("test 4") {
    ASSERT(5 + 5 == 10);
}

// one more false assertion
TEST_CASE("test 5") {
    using std::literals::operator""sv;
    ASSERT("hello"sv == "world"sv);
}