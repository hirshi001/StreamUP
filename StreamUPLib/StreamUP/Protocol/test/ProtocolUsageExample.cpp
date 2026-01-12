#include "Protocol/Protocol.h"

#include <boost/test/unit_test.hpp> // header-only version

BOOST_AUTO_TEST_CASE(ProtocolUsageExample)
{
    // Step 1. Create Protocol object with config
    using namespace SUP;
    Protocol::Config config;
    config.allowInsecureConnections = true;

    auto protocolCreationResult = Protocol::createProtocol(config);
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




    protocol->getConnectionForPacket();
}
