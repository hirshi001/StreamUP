add_library(StreamUPProtocol STATIC
        include/Protocol/packet/Packet.h
        include/Protocol/connection/Connection.h
        include/Protocol/stream/Stream.h
        include/Protocol/connection/ConnectionManager.h
        include/Protocol/connection/ConnectionIdHandler.h)

include(Sources.cmake)

target_include_directories(StreamUPProtocol PUBLIC include)


find_package(OpenSSL REQUIRED)

target_link_libraries(StreamUPProtocol PRIVATE OpenSSL::SSL OpenSSL::Crypto PUBLIC StreamUPData StreamUPPlatform)


add_subdirectory(test)