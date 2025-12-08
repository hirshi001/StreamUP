#pragma once

#include <cstdint>
#include <string>

namespace SUP
{
class Address
{
public:
    Address(const std::string& address, uint16_t port);

    [[nodiscard]] const std::string& getAddress() const;
    [[nodiscard]] uint16_t getPort() const;

private:
    std::string address;
    uint16_t port;
};
}
