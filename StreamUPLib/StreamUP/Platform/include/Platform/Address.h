#pragma once

#include <string>
#include <utility>

namespace SUP
{
class Address
{
public:
    Address();
    Address(std::string address, uint16_t port) : address(std::move(address)), port(port) {}

    [[nodiscard]] const std::string &getAddress() const;

    [[nodiscard]] uint16_t getPort() const;

private:
    std::string address;
    uint16_t port;
};
}
