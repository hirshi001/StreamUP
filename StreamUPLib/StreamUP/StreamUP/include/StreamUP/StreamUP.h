#pragma once

#include "StreamUP/Config.h"
#include "StreamUP/Connection/Connection.h"
#include "StreamUP/ConnectionManager/ConnectionManager.h"

#include <memory>
#include <vector>

namespace SUP
{
enum class InitializationResult
{
    SUCCESS = 0,
    FAILED_TO_CREATE_SOCKET = 1,
};

class StreamUP
{

private:
    explicit StreamUP(const StreamUPConfig &config) : config(config)
    {
    }


    InitializationResult initializeStreamUP()
    {
        connectionManager = std::make_unique<ConnectionManager>(
            config.maxIncomingConnections + config.maxOutgoingConnections);
        localSocket = ;
        if (localSocket == -1)
        {
            return InitializationResult::FAILED_TO_CREATE_SOCKET;
        }

        return InitializationResult::SUCCESS;
    }


public:
    static std::pair<std::optional<StreamUP>, InitializationResult> create(const StreamUPConfig &config)
    {
        StreamUP instance(config);

        InitializationResult result = instance.initializeStreamUP();
        if (result != InitializationResult::SUCCESS)
            return std::make_pair(std::nullopt, result);

        return std::make_pair(std::make_optional(std::move(instance)), result);
    };

    StreamUP(const StreamUP &) = delete;

    StreamUP &operator=(const StreamUP &) = delete;


    StreamUP(StreamUP &&) noexcept = default;

    StreamUP &operator=(StreamUP &&) noexcept = default;

    ~StreamUP() = default;

    [[nodiscard]] int bind(const Address& address)
    {
        return localSocket->bind(address);
    }

    std::pair<std::unique_ptr<Connection>, CreateConnectionResult> createConnection(
        const ConnectionConfig &connectionConfig)
    {
        return connectionManager->createConnection(connectionConfig);
    }

private:
    StreamUPConfig config;
    std::unique_ptr<ConnectionManager> connectionManager;
    std::unique_ptr<Socket> localSocket;
};
}
