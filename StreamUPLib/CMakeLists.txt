cmake_minimum_required(VERSION 3.31)
project(StreamUP)

enable_testing()
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

include(FetchContent)

find_program(IWYU_PATH include-what-you-use)

if(IWYU_PATH)
    set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE
            ${IWYU_PATH}
            -Xiwyu
            --mapping_file=${CMAKE_SOURCE_DIR}/iwyu.imp
            --error
    )
else()
    message("ERROR" FATAL_ERROR)
endif()


set(CMAKE_CXX_STANDARD 20)

add_subdirectory(StreamUP)
