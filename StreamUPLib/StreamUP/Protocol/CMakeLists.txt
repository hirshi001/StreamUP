add_library(StreamUPProtocol STATIC)

include(Sources.cmake)

target_include_directories(StreamUPProtocol PUBLIC include)


find_package(OpenSSL REQUIRED)

target_link_libraries(StreamUPProtocol PRIVATE OpenSSL::SSL OpenSSL::Crypto PUBLIC StreamUPData StreamUPPlatform)


add_subdirectory(test)