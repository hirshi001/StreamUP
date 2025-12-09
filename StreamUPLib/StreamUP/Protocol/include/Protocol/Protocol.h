//
// Created by Hrishikesh Ingle on 6/8/25.
//

#pragma once
#include <vector>

#include <openssl/rand.h>
#include <openssl/ssl.h>

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
    ServerProtocol(
        const std::vector<Security::CipherSuite>& _cipherSuites,
        const std::string &ephemeralGroupName,
        EVP_PKEY *_identityPrivateKey = nullptr,
        EVP_PKEY *_identityPublicKey = nullptr
        )
    {
        this->cipherSuites = _cipherSuites;

        int groupNID = OBJ_sn2nid(ephemeralGroupName.c_str());
        if (groupNID == NID_undef)
        {
            throw std::runtime_error("Unknown ephemeral key group: " + ephemeralGroupName);
        }

        ctx = SSL_CTX_new(TLS_method());
        if (!ctx) throw std::runtime_error("Failed to create SSL_CTX");

        int groups[] = {groupNID};
        if (SSL_CTX_set1_groups(ctx, groups, 1) != 1)
        {
            throw std::runtime_error("Failed to set ephemeral group: " + ephemeralGroupName);
        }

        if (_identityPrivateKey && _identityPublicKey) {
            this->identityPrivateKey = EVP_PKEY_dup(_identityPrivateKey);
            this->identityPublicKey  = EVP_PKEY_dup(_identityPublicKey);
        } else if (_identityPrivateKey || _identityPublicKey) {
            // Only one key is provided — invalid
            throw std::runtime_error(
                "Both identity keys must be either null or non-null; got mismatched nulls");
        }

    }

    ~ServerProtocol()
    {
        SSL_CTX_free(ctx);
        if (identityPrivateKey) EVP_PKEY_free(identityPrivateKey);
        if (identityPublicKey) EVP_PKEY_free(identityPublicKey);
    }

private:
    SSL_CTX *ctx;
    EVP_PKEY *identityPrivateKey;
    EVP_PKEY *identityPublicKey;
    std::vector<Security::CipherSuite> cipherSuites;
};
}

class Protocol
{
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
private:
};
