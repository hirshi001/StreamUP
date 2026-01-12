#include "Protocol/Protocol.h"

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <netdb.h>
#include <string>
#include <stdexcept>
#include <thread>

#include <boost/test/unit_test.hpp> // header-only version

#include <sys/fcntl.h>

int sock;



int create_udp_socket(uint16_t port)
{
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // Bind to local addresses

    addrinfo *result = nullptr;
    int rc = getaddrinfo(nullptr, std::to_string(port).c_str(),
                         &hints, &result);
    if (rc != 0) { throw std::runtime_error(gai_strerror(rc)); }

    int sock = -1;

    for (addrinfo *ai = result; ai != nullptr; ai = ai->ai_next)
    {
        sock = ::socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
        if (sock < 0)
            continue;

        // Optional but recommended: allow fast rebinding
        int yes = 1;
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

        if (::bind(sock, ai->ai_addr, ai->ai_addrlen) == 0)
        {
            break; // success
        }

        ::close(sock);
        sock = -1;
    }

    freeaddrinfo(result);

    if (sock < 0) { throw std::runtime_error("failed to bind UDP socket"); }

    return sock;
}

void set_nonblocking(int sock)
{
    int flags = fcntl(sock, F_GETFL, 0);
    if (flags == -1) { throw std::runtime_error("fcntl(F_GETFL) failed"); }

    if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) == -1) { throw std::runtime_error("fcntl(F_SETFL) failed"); }
}

void sendPacketHandler(std::unique_ptr<SUP::SendPacket> packet)
{
    const auto &buffer = packet->data;
    const auto &address = packet->address;

    auto dst = resolve_address(address.getAddress(), std::to_string(address.getPort()));
    ssize_t bytesSent = sendto(sock, buffer.data, buffer.getWriteIndex(), 0,
                               reinterpret_cast<sockaddr *>(&dst.addr),
                               dst.addrlen);
    if (bytesSent == -1) { std::cerr << "Failed to send packet: " << strerror(errno) << std::endl; }
}


BOOST_AUTO_TEST_CASE(ProtocolUsageExample)
{
    // Step 1. Create Protocol object with config
    using namespace SUP;
    Protocol::Config config;
    config.allowInsecureConnections = true;

    Protocol::Handlers handlers;
    handlers.sendPacketHandler = sendPacketHandler;

    auto protocolCreationResult = Protocol::createProtocol(config, handlers);
    if (!protocolCreationResult)
    {
        switch (protocolCreationResult.error())
        {
            case Protocol::CreateProtocolError::FAILED_TO_CREATE_SSL_CTX:
                std::cout << "Failed to create SSL context" << std::endl;
                break;
            case Protocol::CreateProtocolError::MISSING_PRIVATE_KEY:
                std::cout << "Missing private key" << std::endl;
                break;
            case Protocol::CreateProtocolError::MISSING_PUBLIC_KEY:
                std::cout << "Missing public key" << std::endl;
                break;
            case Protocol::CreateProtocolError::MISSING_CIPHER_SUITES:
                std::cout << "Missing cipher suites" << std::endl;
                break;
            case Protocol::CreateProtocolError::MISSING_EPHEMERAL_GROUPS:
                std::cout << "Missing ephemeral groups" << std::endl;
                break;
        }
        return;
    }

    std::unique_ptr<Protocol> protocol = std::move(protocolCreationResult.value());

    // Step 2. Create socket which receives packets
    sock = create_udp_socket(1234);
    set_nonblocking(sock);

    // Step 3. Event Loop
    std::array<uint8_t, 2048> buf{};
    sockaddr_storage src{};
    socklen_t srclen = sizeof(src);

    using namespace std::chrono;
    using clock = steady_clock;
    using timepoint = clock::time_point;
    timepoint now = clock::now();

    while (true)
    {
        // Advance time
        timepoint newNow = clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            newNow - now
        );
        now = newNow;

        protocol->advance_time(elapsed);

        // receive
        ssize_t n;
        while (true)
        {
            n = recvfrom(sock, buf.data(), sizeof(buf), 0,
                         reinterpret_cast<sockaddr *>(&src), &srclen);

            if (n < 0)
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                    break; // no more packets
                throw std::runtime_error("recvfrom failed");
            }
            auto packet = std::make_unique<SUP::Packet>();
            protocol->getConnectionForPacket()
            handle_packet(buf, n, src);
        }



        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }


    protocol->getConnectionForPacket();
}
