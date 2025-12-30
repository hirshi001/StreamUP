target_sources(StreamUPProtocol PUBLIC
        include/Protocol/congestioncontrol/CongestionControl.h
        include/Protocol/congestioncontrol/TCPCongestionControl.h
        include/Protocol/packet/SendPacket.h
        include/Protocol/packet/SendPacketPool.h
        include/Protocol/rtt/DefaultRTTEstimator.h
        include/Protocol/rtt/RTTEstimator.h
        include/Protocol/AuthEncryptionTypes.h
        include/Protocol/Protocol.h
)

target_sources(StreamUPProtocol PRIVATE
        src/rtt/DefaultRTTEstimator.cpp
)