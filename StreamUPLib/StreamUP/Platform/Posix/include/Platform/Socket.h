#pragma once

#include <cstdio>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "Platform/Address.h"
#include "Platform/SocketInterface.h"

namespace SUP
{
class Socket : public SocketInterface
{
public:
    Socket()
    {
        localSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (localSocket < 0)
        {
            perror("socket");
        }
    }

    ~Socket();

    Socket(const Socket &) = delete;

    Socket &operator=(const Socket &) = delete;

    Socket(Socket &&) = delete;

    Socket &operator=(Socket &&) = delete;

    [[nodiscard]] int bind(const Address &address) // NOLINT(*-make-member-function-const)
    {
        if (localSocket < 0)
        {
            perror("socket");
            return 1;
        }
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(address.getPort());

        // Convert IP address string to binary form
        if (inet_pton(AF_INET, address.getAddress().c_str(), &addr.sin_addr) <= 0)
        {
            perror("inet_pton");
            return 1;
        }

        // Bind socket to the address
        if (::bind(localSocket, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0)
        {
            perror("bind");
            return 1;
        }

        return 0; // success
    }

    void sendTo(const char *buffer, int length, const Address &address)
    {
        sockaddr_in dest{};

        ssize_t sent = sendto(localSocket, buffer, length, 0, reinterpret_cast<sockaddr *>(&dest), sizeof(dest));
    }

private:
    int localSocket;
};
}
