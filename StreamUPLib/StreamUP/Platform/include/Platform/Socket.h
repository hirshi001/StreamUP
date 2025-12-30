#pragma once
#include <memory>

#include "Platform/Address.h"


namespace SUP {

template<typename T>
concept SocketImplConcept =
    requires(T t, const Address& addr,
             const char* buf, int len) {
    { t.bind(addr) } -> std::same_as<int>;
    { t.sendTo(buf, len, addr) } -> std::same_as<void>;
             };

template<SocketImplConcept SocketImpl>
class Socket
{

public:
    Socket()
    {
        impl = std::make_unique<SocketImpl>();
    }

    int bind(const Address &address)
    {
        return impl->bind(address);
    }


    int sendTo(const uint8_t* buffer, int len, const Address &address)
    {
        return impl->sendTo(buffer, len);
    }

private:
    std::unique_ptr<SocketImpl> impl;
};

}
