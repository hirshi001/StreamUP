add_library(StreamUP STATIC
        ../Data/src/Placeholder.cpp)

target_include_directories(StreamUP PUBLIC include)
include(Sources.cmake)

add_subdirectory(test)