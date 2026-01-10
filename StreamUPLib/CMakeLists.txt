cmake_minimum_required(VERSION 3.31)
project(StreamUP)

enable_testing()
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

include(FetchContent)

find_program(IWYU_PATH include-what-you-use REQUIRED)

set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE
        ${IWYU_PATH}
        -Xiwyu
        --mapping_file=${CMAKE_SOURCE_DIR}/iwyu.imp
        --error
)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(StreamUP)
