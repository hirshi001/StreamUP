#pragma once

#include "StreamUP/Connection/Connection.h"

#include <cstdint>

namespace SUP
{

enum class CreateConnectionResult
{
    SUCCESS,
    TOO_MANY_CONNECTIONS
};

class ConnectionManager
{

public:
    explicit ConnectionManager(uint64_t maxConnections);

    std::pair<std::unique_ptr<Connection>, CreateConnectionResult> createConnection(const ConnectionConfig& config)
    {
        Connection::Id id = nextConnectionIdAtomic();
        auto connection = std::make_unique<Connection>(id, config, this);
        return {std::move(connection), CreateConnectionResult::SUCCESS};
    }

private:
    Connection::Id nextConnectionIdAtomic()
    {
        return connectionIdCounter.fetch_add(1);
    }

    std::atomic<Connection::Id> connectionIdCounter;
};

}
