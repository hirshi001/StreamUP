#pragma once

#include "Data/buffer/WriteBufferWrapper.h"
#include "Platform/Address.h"

#include <vector>

namespace SUP
{
struct SendPacket
{
    SendPacket(SendPacket &) = delete;

    SendPacket(SendPacket &&) = delete;

    SendPacket &operator=(const SendPacket &) = delete;

    SendPacket &operator=(SendPacket &&) = delete;

    explicit SendPacket(int size)
        : buffer(size), data(buffer.data(), size) {}


    [[nodiscard]] size_t bufferCapacity() const { return buffer.size(); }

private:
    std::vector<uint8_t> buffer;

public:
    Address address;
    BufferUtil::WriteBufferWrapper data;
};
}
