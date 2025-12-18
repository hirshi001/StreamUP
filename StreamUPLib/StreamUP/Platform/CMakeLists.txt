add_library(StreamUPPlatform STATIC
        include/Platform/Socket.h)

target_include_directories(StreamUPPlatform PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
include(Sources.cmake)

if(UNIX OR APPLE)
    add_subdirectory(Posix)
else()
    message(FATAL_ERROR "Non Posix versions have not been implemented yet")
endif()