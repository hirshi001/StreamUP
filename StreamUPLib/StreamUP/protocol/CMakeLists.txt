add_library(protocol STATIC)

target_sources(protocol PRIVATE
        Protocol.cpp
)

target_include_directories(protocol PUBLIC include)