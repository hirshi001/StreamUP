cmake_minimum_required(VERSION 3.31)
project(StreamUP)

enable_testing()
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(StreamUP)
