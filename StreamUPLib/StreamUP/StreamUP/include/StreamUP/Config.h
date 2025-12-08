#pragma once
#include <climits>
#include <cstdint>

namespace SUP
{
struct StreamUPConfig
{
    StreamUPConfig(uint64_t maxIncomingConnections, uint64_t maxOutgoingConnections,
                   uint64_t maxStreamsPerConnection, uint64_t maxMessageSize)
        : maxIncomingConnections(maxIncomingConnections),
          maxOutgoingConnections(maxOutgoingConnections),
          maxStreamsPerConnection(maxStreamsPerConnection),
          maxMessageSize(maxMessageSize)
    {
    }

    uint64_t bitsForId = sizeof(uint8_t) * CHAR_BIT;
    uint64_t maxIncomingConnections;
    uint64_t maxOutgoingConnections;
    uint64_t maxStreamsPerConnection;
    uint64_t maxMessageSize;
};

enum Reliability
{
    RELIABLE_ORDERED,
    RELIABLE_UNORDERED,
    UNRELIABLE_ORDERED,
    UNRELIABLE_UNORDERED,
    UNRELIABLE_LATEST_MESSAGES_RELIABLE,
    NUM_RELIABILITY_MODES
};

struct ConnectionConfig
{
    ConnectionConfig(uint64_t maxStreams,
                     uint64_t maxMessageSize) : maxStreams(maxStreams),
                                                maxMessageSize(maxMessageSize)
    {
    }

    uint64_t maxStreams;
    uint64_t maxMessageSize;
};


struct StreamConfig
{
    StreamConfig(Reliability reliability, uint64_t bufferSize) : reliability(reliability),
                                                                 bufferSize(bufferSize)
    {
    }

    Reliability reliability;
    uint64_t bufferSize;
};
}
