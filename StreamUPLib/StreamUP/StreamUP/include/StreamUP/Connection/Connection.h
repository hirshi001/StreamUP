//
// Created by Hrishikesh Ingle on 7/16/25.
//

#pragma once
#include <cassert>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <vector>
#include <boost/mpl/assert.hpp>

#include "../../../../Platform/include/Platform/Address.h"
#include "StreamUP/ConnectionManager/ConnectionManager.h"


namespace SUP
{
/**
 * @brief Result values for creating streams.
 */
enum class QueueStreamCreationResult
{
    PENDING,
    ALREADY_IN_USE,
    INVALID_STREAM_ID
};


class Connection
{
public:
    /// @brief The type of the identifier used for a connection on this local system.
    using Id = uint64_t;
    using StreamId = uint64_t;

    Connection(Id id, const ConnectionConfig &config) : id(id), config(config), connectedAddress("", 0)
    {
        streams.resize(config.maxStreams);
    }

    void connect(const Address &address)
    {
        // TODO: Check validity of address
        connectedAddress = address;
        connected = true;
    }

    /**
     * Attempts to create a stream with the given stream id. Returns @ref SUP::QueueStreamCreationResult::PENDING "PENDING" if the
     * connection will attempt to create a stream. If the stream id is in use,
     * @ref SUP::QueueStreamCreationResult::ALREADY_IN_USE "ALREADY_IN_USE" will be returned. If the provided stream ID is invalid,
     * @ref SUP::QueueStreamCreationResult::INVALID_STREAM_ID "INVALID_STREAM_ID" will be returned
     * @return The result of the attempt to create a stream.
     */
    QueueStreamCreationResult createStream(StreamId streamId)
    {
        if (streamId >= config.maxStreams)
            return QueueStreamCreationResult::INVALID_STREAM_ID;

        {
            auto& [stream, mutex] = streams[streamId];
            std::lock_guard lock(mutex);
            if (stream != nullptr)
                return QueueStreamCreationResult::ALREADY_IN_USE;

            stream = std::make_unique<Stream>(streamId);
            stream->initialize();
        }

        return QueueStreamCreationResult::PENDING;
    }

    int getCongestionWindow();



private:

    Id id;
    ConnectionConfig config;

    bool connected = false;
    Address connectedAddress;

    using Stream = ConnectionConfig;

    using StreamPtr = std::unique_ptr<Stream>;
    using StreamHandle = std::pair<StreamPtr, std::mutex>;
    std::vector<StreamHandle> streams;

};
}
