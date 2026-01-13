#include "Protocol/Protocol.h"

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <netdb.h>
#include <string>
#include <stdexcept>
#include <thread>

#include <asio.hpp>
#include <boost/test/unit_test.hpp> // header-only version

#include <sys/fcntl.h>
struct SocketData
{
    SocketData() : socket(io_context) {}

    asio::io_context io_context;
    asio::ip::udp::socket socket;
};


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
    SocketData sockData;
    sockData.socket.open(asio::ip::udp::v6());

    asio::ip::v6_only dualStack(false);
    sockData.socket.set_option(dualStack);

    asio::ip::udp::endpoint localEndpoint(asio::ip::udp::v6(), 12345);
    sockData.socket.bind(localEndpoint);

    // Step 3. Event Loop
    std::array<uint8_t, 2048> buf{};
    asio::ip::udp::endpoint src;

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
        size_t n;
        while (true)
        {
            n = sockData.socket.receive_from(asio::buffer(buf.data(), buf.size()), src);

            if (n < 0)
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                    break; // no more packets
                throw std::runtime_error("recvfrom failed");
            }
            src.
            auto packet = std::make_unique<SUP::Packet>();
            protocol->getConnectionForPacket()
            handle_packet(buf, n, src);
        }



        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }


    protocol->getConnectionForPacket();
}
