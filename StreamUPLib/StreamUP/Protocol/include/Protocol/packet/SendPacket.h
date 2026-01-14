#pragma once

#include "Data/buffer/WriteBufferWrapper.h"

#include <vector>

namespace SUP
{

template <typename Endpoint>
struct SendPacket
{
    SendPacket(SendPacket &) = delete;

    SendPacket(SendPacket &&) = delete;

    SendPacket &operator=(const SendPacket &) = delete;

    SendPacket &operator=(SendPacket &&) = delete;

    explicit SendPacket(int size)
        : buffer(size), address(), data(buffer.data(), size) {}


    [[nodiscard]] size_t bufferCapacity() const { return buffer.size(); }

private:
    std::vector<uint8_t> buffer;

public:
    Endpoint address;
    BufferUtil::WriteBufferWrapper data;
};
}
