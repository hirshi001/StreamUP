#pragma once

#include "StreamUP/Config.h"
#include "Connection.h"

#include <cstdint>
#include <memory>

namespace SUP
{

class StreamUP
{

public:
    using Id = uint64_t;

    explicit StreamUP(const StreamUPConfig &config);

    ~StreamUP();

    // Add methods and members as needed for the SUP functionality
    std::unique_ptr<Connection> createConnection(consts ConnectionConfig& config);

    std::unique_ptr<Connection> createStream(Id id, const StreamConfig& config);

    void createStream(const Id id, const StreamConfig& config);

private:
    class StreamUPImpl;
    std::unique_ptr<StreamUPImpl> impl;

    StreamUPConfig config;
};

}