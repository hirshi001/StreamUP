#pragma once

#include <utility>
#include <vector>

namespace SUP
{
template<typename Endpoint>
struct Packet
{
    explicit Packet(const Endpoint &endpoint, const uint8_t *buffer, size_t length) // NOLINT
        : endpoint(endpoint), buffer(buffer), length(length) {}

    Endpoint endpoint;
    const uint8_t *buffer;
    size_t length;
};
}
