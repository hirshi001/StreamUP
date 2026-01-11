#pragma once

#include "Protocol/stream/Stream.h"

#include <map>
#include <vector>

namespace SUP
{
class Connection
{
public:
    using ConnectionId = uint64_t;
    using StreamId = uint32_t; // TODO: Make StreamId VarInt
    std::vector<ConnectionId> connectionIds;
    std::map<StreamId, Stream> streams;

    void addConnectionId(ConnectionId connectionId)
    {
        connectionIds.push_back(connectionId);
    }
};
}