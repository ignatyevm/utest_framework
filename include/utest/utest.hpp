#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <functional>

namespace utest {
    namespace detail {


            using test_func = std::function<void(std::string_view)>;
            using test_case = std::pair<std::string_view, test_func>;


        struct assertion {
            assertion(std::string_view condition, bool condition_value, std::string_view file, int line,
                      std::string_view message)
                : condition(condition), condition_value(condition_value), file(file), line(line), message(message) {}
            std::string_view what_condition() const { return condition; }
            bool is_false() const { return condition_value; }
            std::string_view what_file() const { return file; }
            int what_line() const { return line; }
            std::string_view what_message() const { return message; }
        private:
            std::string_view condition;
            bool condition_value;
            std::string_view file;
            int line;
            std::string_view message;
        };


        inline std::vector<test_case> tests;
        inline std::map<std::string_view, std::vector<assertion>> assertions;

        struct auto_test_register {
            auto_test_register(std::string_view name, const test_func& func) {
                tests.emplace_back(name, func);
                assertions.emplace(name, std::vector<assertion>());
            }
        };

        namespace {
            void register_assert(std::string_view test_name, std::string_view condition, bool condition_value,
                                 std::string_view file, int line, std::string_view message = "") {
                assertions[test_name].emplace_back(condition, condition_value, file, line, message);
            }
            void print_assertion_info(const detail::assertion& fail) noexcept {
                std::cerr << "Assertion failed in " << fail.what_file() << ":" << fail.what_line() << ": "
                          << fail.what_condition() << std::endl;
                if (!fail.what_message().empty()) {
                    std::cerr << "    With message: " << fail.what_message() << std::endl;
                }
            }
        }
    }

    namespace {
        int run_tests() {
            int passed_tests = 0;
            // register all assertions
            for (const auto&[name, func] : detail::tests) {
                std::invoke(func, name);
            }
            for (const auto&[name, assertions] : detail::assertions) {
                // store failed asserts
                std::vector<std::reference_wrapper<const detail::assertion>> fails;
                fails.reserve(assertions.size());
                for (const auto& assertion : assertions) {
                    if (!assertion.is_false()) {
                        fails.emplace_back(std::cref(assertion));
                    }
                }
                std::cerr << "Running " << name << "...";
                if (fails.empty()) {
                    std::cerr << " OK!";
                    ++passed_tests;
                }
                std::cerr << std::endl;
                // for each fail prints the fail info (fails.size() > 0)
                for (const auto& fail : fails) {
                    detail::print_assertion_info(fail);
                }
            }
            std::cerr << "========== Passed " << passed_tests << "/" << detail::tests.size() << " tests ==========" << std::endl;
            return static_cast<int>(detail::tests.size()) - passed_tests;
        }
    }
}

// magic macro sandwich
#define TEST_CASE(name) __TEST_CASE_2(name, __LINE__)
#define __TEST_CASE_2(name, line) __TEST_CASE_3(name, line)
#define __TEST_CASE_3(name, line) __TEST_CASE_4(name, utest_test_case_##line, line)
#define __TEST_CASE_4(name, func_name, line) \
    static void func_name(std::string_view test_name); \
    static utest::detail::auto_test_register test_register_##line(name, &func_name); \
    static void func_name(std::string_view test_name)

#define ASSERT(...) UNWIND_ASSERT(__VA_ARGS__, ASSERT_MESSAGE, ASSERT_DEFAULT)(__VA_ARGS__)
#define UNWIND_ASSERT(condition, message, handler, ...) handler

#define ASSERT_DEFAULT(condition) ASSERT_MESSAGE(condition, "")
#define ASSERT_MESSAGE(condition, message) \
    utest::detail::register_assert(test_name, #condition, condition, __FILE__, __LINE__, message)

#define DEFINE_TEST_RUNNER_HERE() int main(int argc, char** argv) { return utest::run_tests(); }