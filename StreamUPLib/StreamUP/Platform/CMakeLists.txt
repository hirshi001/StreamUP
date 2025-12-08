add_library(StreamUPPlatform STATIC)
add_library(StreamUP::Platform ALIAS StreamUPPlatform)

target_include_directories(StreamUPPlatform PUBLIC include)
include(Sources.cmake)

if(UNIX OR APPLE)
    add_subdirectory(Posix)
else()
    message(FATAL_ERROR "Non Posix versions have not been implemented yet")
endif()