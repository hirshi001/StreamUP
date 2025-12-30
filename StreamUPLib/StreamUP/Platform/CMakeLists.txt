add_library(StreamUPPlatform INTERFACE
        test/AddressTest.cpp)

include(Sources.cmake)

target_include_directories(StreamUPPlatform INTERFACE include)

if(UNIX OR APPLE)
    # add_subdirectory(Posix)
else()
    message(FATAL_ERROR "Non Posix versions have not been implemented yet")
endif()

add_subdirectory(test)