add_library(StreamUPData INTERFACE)

include(Sources.cmake)

target_include_directories(StreamUPData INTERFACE include)

add_subdirectory(test)