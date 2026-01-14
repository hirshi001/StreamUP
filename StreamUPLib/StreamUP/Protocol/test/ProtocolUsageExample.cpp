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

using namespace SUP;
using AddressT = asio::ip::udp::endpoint;
using SendPacketT = SendPacket<AddressT>;
using PacketT = Packet<AddressT>;
using ProtocolT = Protocol<AddressT>;

struct SocketData
{
    SocketData() : socket(io_context) {}

    asio::io_context io_context;
    asio::ip::udp::socket socket;
};


void sendPacketHandler(std::shared_ptr<SocketData> sockData, std::unique_ptr<SendPacketT> packet)
{
    const auto &buffer = packet->data;
    const auto &dst = packet->address;
    try
    {
        size_t bytesSent = sockData->socket.send_to(asio::const_buffer(buffer.data, buffer.getWriteIndex()), dst);
        assert(bytesSent == buffer.getWriteIndex());
    }
    catch (const asio::system_error &e) { std::cerr << "Failed to send packet: " << e.what() << std::endl; }
}


BOOST_AUTO_TEST_CASE(ProtocolUsageExample)
{
    // Step 1. Create and bind the socket
    std::shared_ptr<SocketData> sockData;
    sockData->socket.open(asio::ip::udp::v6());

    asio::ip::v6_only dualStack(false);
    sockData->socket.set_option(dualStack);

    sockData->socket.non_blocking(true);

    asio::ip::udp::endpoint localEndpoint(asio::ip::udp::v6(), 12345);
    sockData->socket.bind(localEndpoint);

    // Step 2. Create Protocol object with config
    using namespace SUP;
    ProtocolT::Config config;
    config.allowInsecureConnections = true;

    ProtocolT::Handlers handlers;
    handlers.sendPacketHandler = [sockData](std::unique_ptr<SendPacketT> packet)
    {
        sendPacketHandler(sockData, std::move(packet));
    };

    auto protocolCreationResult = ProtocolT::createProtocol(config, handlers);
    if (!protocolCreationResult)
    {
        switch (protocolCreationResult.error())
        {
            case ProtocolT::CreateProtocolError::FAILED_TO_CREATE_SSL_CTX:
                std::cout << "Failed to create SSL context" << std::endl;
                break;
            case ProtocolT::CreateProtocolError::MISSING_PRIVATE_KEY:
                std::cout << "Missing private key" << std::endl;
                break;
            case ProtocolT::CreateProtocolError::MISSING_PUBLIC_KEY:
                std::cout << "Missing public key" << std::endl;
                break;
            case ProtocolT::CreateProtocolError::MISSING_CIPHER_SUITES:
                std::cout << "Missing cipher suites" << std::endl;
                break;
            case ProtocolT::CreateProtocolError::MISSING_EPHEMERAL_GROUPS:
                std::cout << "Missing ephemeral groups" << std::endl;
                break;
        }
        return;
    }

    std::unique_ptr<Protocol<AddressT> > protocol = std::move(protocolCreationResult.value());

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
            try { n = sockData->socket.receive_from(asio::buffer(buf.data(), buf.size()), src); }
            catch (const asio::system_error &e)
            {
                if (e.code() == asio::error::would_block)
                    break; // No more data to read
                std::cerr << "Failed to receive from: " << e.what() << std::endl;
            }

            Packet packet(src, buf.data(), n);
            auto connection = protocol->getConnectionForPacket(packet);
            connection->handlePacket(packet);
        }


        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
