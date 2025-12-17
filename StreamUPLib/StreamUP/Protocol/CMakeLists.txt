add_library(StreamUPProtocol STATIC
        include/Protocol/congestioncontrol/CongestionControl.h
        include/Protocol/congestioncontrol/TCPCongestionControl.h
        include/Protocol/rtt/RTTEstimator.h
        include/Protocol/rtt/DefaultRTTEstimator.h
        include/Protocol/AuthEncryptionTypes.h
        include/Protocol/packet/SendPacket.h
        src/rtt/DefaultRTTEstimator.cpp
        include/Protocol/packet/SendPacketPool.h)

target_include_directories(StreamUPProtocol PUBLIC include)


find_package(OpenSSL REQUIRED)

target_link_libraries(StreamUPProtocol PRIVATE OpenSSL::SSL OpenSSL::Crypto PUBLIC StreamUPData StreamUPPlatform)


add_subdirectory(test)