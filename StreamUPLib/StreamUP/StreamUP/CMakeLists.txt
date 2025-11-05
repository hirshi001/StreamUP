add_library(StreamUP STATIC)

target_sources(StreamUP PRIVATE
    StreamUP.cpp
)
target_include_directories(StreamUP PUBLIC include)
include(Sources.cmake)


add_subdirectory(test)