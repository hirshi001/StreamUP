#include "StreamUP/StreamUP.h"

namespace SUP
{
StreamUP::StreamUP(const StreamUPConfig &config) : config(config)
{
}

StreamUP::~StreamUP()
= default;

std::pair<StreamUP::ConnectionPtr, CreateConnectionResult> StreamUP::createConnection(const ConnectionConfig& connectionConfig)
{
    if (numberOutgoingConnections() == config.maxOutgoingConnections())
    {
        return {ConnectionPtr(), CreateConnectionResult::TOO_MANY_CONNECTIONS};
    }
    Connection::Id id = next_connection_id_atomic();
    ConnectionPtr connection = std::make_unique<Connection>(id, connectionConfig);

    return {std::move(connection), CreateConnectionResult::SUCCESS};
}
}
