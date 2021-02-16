cmake_minimum_required(VERSION 3.17)
project(utest_framework VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_library(utest_framework INTERFACE)
target_include_directories(utest_framework INTERFACE include/)