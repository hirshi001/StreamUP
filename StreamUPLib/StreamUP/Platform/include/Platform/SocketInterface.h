#pragma once

#include "Platform/Address.h"

namespace SUP
{
class SocketInterface
{
public:
    int bind(const Address &address);
    void sendTo(const char *buffer, int length, const Address &address);

};
}
