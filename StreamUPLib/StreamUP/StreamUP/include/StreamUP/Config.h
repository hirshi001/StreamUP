#pragma once

namespace SUP
{

struct StreamUPConfig
{
    size_t bitsForId = sizeof(uint8_t) * CHAR_BIT;
    size_t maxIncomingConnections;
    size_t maxOutgoingConnections;
    size_t maxStreamsPerConnection;
    size_t maxMessageSize;
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
    size_t maxStreams;
    size_t maxMessageSize;
};


struct StreamConfig
{
    Reliability reliability;
    size_t bufferSize = 0;
};

}