add_library(StreamUPData STATIC
        include/Data/buffer/BufferUtil.h
        include/Data/buffer/ReadBufferWrapper.h
        include/Data/buffer/WriteBufferWrapper.h)

target_sources(StreamUPData PRIVATE
        src/Placeholder.cpp
)

target_include_directories(StreamUPData PUBLIC include)

add_subdirectory(test)