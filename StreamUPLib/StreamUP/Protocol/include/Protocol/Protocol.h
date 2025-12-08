//
// Created by Hrishikesh Ingle on 6/8/25.
//

#pragma once
#include <vector>

#include <openssl/rand.h>

#include "AuthEncryptionTypes.h"


namespace SUP
{

enum class SUPVersions
{
    V1 = 1
};


class ServerProtocol
{
public:

    ServerProtocol() {
    }

    void acceptClientHello(std::vector<uint8_t> data)
    {
        int numAvailableVersion;
    }
};
}
class Protocol {
public:
    void clientHandshake();
    void serverListener();

    /**
     * Creates and sends a handshake packet to the server
     * @return
     */
    // HandshakeData generateHandshakePacket();


    /**
     * Creates and multiple handshake packets of the given sizes to the server
     * @return
     */
    // HandshakeData generateHandshakePacket(std::vector<int> sizes);


};
