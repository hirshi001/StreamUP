#pragma once

#include "Protocol/packet/Packet.h"
#include "Protocol/stream/Stream.h"

#include <map>
#include <vector>

namespace SUP
{
template <typename T>
concept ByteArray =
    requires {
    typename T::value_type;
    T{}.size();
    } &&
    std::is_same_v<T, std::array<typename T::value_type, T{}.size()>> &&
    std::is_same_v<typename T::value_type, std::uint8_t>;

template<typename Address>
class Connection
{
public:
    static constexpr int CONNECTION_ID_SIZE = 12;
    using ConnectionId = std::array<uint8_t, CONNECTION_ID_SIZE>;
    static_assert(ByteArray<ConnectionId>);

    using StreamId = uint32_t; // TODO: Make StreamId VarInt
    std::vector<ConnectionId> connectionIds;
    std::map<StreamId, Stream> streams;
    Address remoteEndpoint;

    void handlePacket(const Packet<Address> &packet)
    {

    }

    void addConnectionId(ConnectionId connectionId)
    {
        connectionIds.push_back(connectionId);
    }
};
}