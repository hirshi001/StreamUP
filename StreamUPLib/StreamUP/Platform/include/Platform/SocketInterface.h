#pragma once

#include "StreamUP/Address.h"

namespace SUP
{
class SocketInterface
{

    int bind(const Address &address);
    void sendTo(const char *buffer, int length, const Address &address);

};
}
