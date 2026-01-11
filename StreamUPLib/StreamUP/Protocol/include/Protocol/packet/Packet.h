#pragma once

#include "Platform/Address.h"

#include <utility>
#include <vector>

namespace SUP
{
struct Packet
{
    Packet(Packet &) = delete;

    Packet(Packet &&) = delete;

    Packet &operator=(const Packet &) = delete;

    Packet &operator=(Packet &&) = delete;

    explicit Packet(const Address &address, const std::vector<uint8_t>& buffer) // NOLINT
        : address(address), buffer(buffer) {}


    [[nodiscard]] size_t bufferCapacity() const { return buffer.size(); }

    Address address;
    std::vector<uint8_t> buffer;
};
}
