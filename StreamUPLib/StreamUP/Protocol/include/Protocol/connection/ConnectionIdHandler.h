#pragma once

#include "Protocol/connection/Connection.h"

#include <openssl/rand.h>

namespace SUP
{
class ConnectionIdHandler
{
public:

    ConnectionIdHandler() = default;
    Connection::ConnectionId getNewConnectionId()
    {
        Connection::ConnectionId connectionId;
        RAND_bytes(connectionId.data(), connectionId.size());
        return connectionId;
    }

private:

};
}
