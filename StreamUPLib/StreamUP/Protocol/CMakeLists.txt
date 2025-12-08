add_library(StreamUPProtocol STATIC
        include/Protocol/congestioncontrol/CongestionControl.h
        include/Protocol/congestioncontrol/TCPCongestionControl.h
        include/Protocol/rtt/RTTEstimator.h
        include/Protocol/rtt/DefaultRTTEstimator.h
        src/rtt/DefaultRTTEstimator.cpp
        test/TCPTest.cpp
        include/Protocol/AuthEncryptionTypes.h
        include/Protocol/buffer/ArrayBuffer.h)

target_sources(StreamUPProtocol PRIVATE
        Protocol.cpp
)


find_package(OpenSSL REQUIRED)

target_link_libraries(StreamUPProtocol PRIVATE OpenSSL:SSL OpenSSL::Crypto)

target_include_directories(StreamUPProtocol PUBLIC include)

add_subdirectory(test)