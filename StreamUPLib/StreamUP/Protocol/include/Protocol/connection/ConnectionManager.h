#pragma once

#include <algorithm>

#include <Protocol/connection/Connection.h>

namespace SUP
{
class ConnectionManager
{
public:
    ConnectionManager() = default;

    Connection *getConnection(Connection::ConnectionId id)
    {
        const auto connectionIter = connections.find(id);
        if (connectionIter == connections.end())
            return nullptr;


        auto* connection = connectionIter->second;
        auto& connectionIds = connection->connectionIds;

        if (std::ranges::find(connectionIds, id) == connectionIds.end())
            return nullptr;


        return connection;
    }

    Connection* addNewConnection()
    {
        const auto id = connectionIdHandler.getNewConnectionId();
        const auto connection = new Connection();
        connection->addConnectionId(id);
        connections[id] = connection;
        return connection;
    }

private:
    std::map<Connection::ConnectionId, Connection *> connections;
    ConnectionIdHandler connectionIdHandler;
};
}
