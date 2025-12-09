add_library(StreamUPData STATIC
        include/Data/buffer/BufferUtil.h)

target_sources(StreamUPData PRIVATE
        src/Placeholder.cpp
)

target_include_directories(StreamUPData PUBLIC include)

add_subdirectory(test)