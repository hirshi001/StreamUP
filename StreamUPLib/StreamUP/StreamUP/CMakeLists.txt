add_library(StreamUP STATIC)

target_include_directories(StreamUP PUBLIC include)
include(Sources.cmake)

add_subdirectory(test)