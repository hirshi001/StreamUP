#pragma once

#include "Data/buffer/ReadBufferWrapper.h"
#include "Platform/SocketInterface.h"
#include "Protocol/AuthEncryptionTypes.h"
#include "Protocol/packet/SendPacket.h"
#include "Protocol/packet/SendPacketPool.h"

#include <openssl/ssl.h>
#include <openssl/types.h>


namespace SUP
{

enum class SUPVersions
{
    V1 = 1
};

struct IdentityKeys
{
    EVP_PKEY *identityPrivateKey;
    EVP_PKEY *identityPublicKey;
};

class Protocol
{
    static constexpr int MAX_NUMBER_NEGOTIABLES = 16;

public:
    Protocol(Protocol &) = delete;

    Protocol(Protocol &&) = delete;

    Protocol &operator=(Protocol &) = delete;

    Protocol &operator=(Protocol &&) = delete;

    Protocol(
        const std::vector<Security::CipherSuite> &_cipherSuites,
        const std::vector<Security::EphemeralKeyGroup> &supportedEphemeralGroups,
        const std::optional<IdentityKeys> &identityKey,
        bool allowInsecureConnections = false
    )
    {
        sendPacketPool.addClassSize(1200);
        this->cipherSuitesAvailable = _cipherSuites;
        this->supportedEphemeralGroups = supportedEphemeralGroups;
        this->allowInsecureConnections = allowInsecureConnections;

        ctx = SSL_CTX_new(TLS_method());
        if (!ctx)
            throw std::runtime_error("Failed to create SSL_CTX");

        if (identityKey)
        {
            this->identityPrivateKey = EVP_PKEY_dup(identityKey->identityPrivateKey);
            this->identityPublicKey = EVP_PKEY_dup(identityKey->identityPublicKey);
        }
    }

    ~Protocol()
    {
        SSL_CTX_free(ctx);
        if (identityPrivateKey)
            EVP_PKEY_free(identityPrivateKey.value());
        if (identityPublicKey)
            EVP_PKEY_free(identityPublicKey.value());
    }

    void acceptPacket(const Address &address, uint8_t *data, int length) {}


    void acceptClientHandshake(const Address &address, uint8_t *data, int length)
    {
        BufferUtil::ReadBufferWrapper buffer(data, length);

        if (!buffer.ensureReadableBytes(sizeof(uint8_t)))
            return AcceptClientHandshakeResult::ILL_FORMED_HANDSHAKE;
        const auto numSUPVersions = buffer.read<uint8_t>();
        if (numSUPVersions >= MAX_NUMBER_NEGOTIABLES)
            return AcceptClientHandshakeResult::ILL_FORMED_HANDSHAKE;

        if (!buffer.ensureReadableBytes(sizeof(uint8_t) * numSUPVersions))
            return AcceptClientHandshakeResult::ILL_FORMED_HANDSHAKE;
        std::vector<uint8_t> supVersions(numSUPVersions);
        for (int i = 0; i < numSUPVersions; i++) { buffer.read(supVersions[i]); }

        if (!buffer.ensureReadableBytes(sizeof(uint8_t)))
            return AcceptClientHandshakeResult::ILL_FORMED_HANDSHAKE;
        const auto numCipherSuites = buffer.read<uint8_t>();
        if (numCipherSuites >= MAX_NUMBER_NEGOTIABLES)
            return AcceptClientHandshakeResult::ILL_FORMED_HANDSHAKE;

        if (!buffer.ensureReadableBytes(sizeof(uint16_t) * numCipherSuites))
            return AcceptClientHandshakeResult::ILL_FORMED_HANDSHAKE;


        std::vector<Security::CipherSuite> cipherSuitesNegotiated;
        cipherSuitesNegotiated.reserve(numCipherSuites);
        for (int i = 0; i < numCipherSuites; i++)
        {
            std::optional<Security::CipherSuite> cipher = Security::idToCipherSuite(i);
            if (!cipher)
                continue;
            cipherSuitesNegotiated.push_back(*cipher);
        }

        if (numCipherSuites == 0 && !allowInsecureConnections)
        {
            // TODO: Return error message saying "Requires Secure Connection" or something along those lines
            return AcceptClientHandshakeResult::SECURE_CONNECTION_REQUIRED;
        }

        if (numCipherSuites != 0) // read Ephemeral Key Data
        {
            if (!buffer.ensureReadableBytes(sizeof(uint8_t)))
                return;
            const uint8_t numClientEphemeralKeys = buffer.read<uint8_t>();
            if (numClientEphemeralKeys >= MAX_NUMBER_NEGOTIABLES)
                return;

            int keyGroupRank = -1;
            Security::EphemeralKeyGroup choosenKeyGroup;
            std::array<uint8_t, Security::MAX_EPHEMERAL_KEY_VALUE_LENGTH> ephemeralKey;
            for (int i = 0; i < numClientEphemeralKeys; i++)
            {
                if (!buffer.ensureReadableBytes(sizeof(uint8_t) + sizeof(uint16_t)))
                    return;

                const uint8_t ephemeralKeyTypeInt = buffer.read<uint8_t>();
                const uint16_t ephemeralKeyLength = buffer.read<uint16_t>();

                if (ephemeralKeyLength > Security::MAX_EPHEMERAL_KEY_VALUE_LENGTH) // Must reject ill-formed hellos
                    return;

                if (!buffer.ensureReadableBytes(ephemeralKeyLength))
                    return;
                buffer.readToArray(ephemeralKey.data(), ephemeralKeyLength);

                std::optional<Security::EphemeralKeyGroup> group = Security::getEphemeralKeyGroup(
                    ephemeralKeyTypeInt);
                if (!group)
                    continue;

                int rank = ephemeralGroupRank(*group);
                if (rank == -1)
                    continue;

                if (keyGroupRank == -1 || rank < keyGroupRank)
                {
                    keyGroupRank = rank;
                    choosenKeyGroup = *group;
                }
            }

            if (keyGroupRank == -1)
            {
                // TODO: Must send back a packet rejecting the handshake OR giving a list of supported ephemeral keys
                return;
            }
        }
    }

private:
    /**
     * Finds the ranking of the provided group. The server wants lower ranking groups compared to higher ranking groups.
     * @param group the group to find the ranking of.
     * @return a number representing the rank, or -1 if not found. Lower rank means higher priority
     */
    int ephemeralGroupRank(Security::EphemeralKeyGroup group)
    {
        for (int i = 0; i < supportedEphemeralGroups.size(); i++)
        {
            if (group == supportedEphemeralGroups[i])
                return i;
        }
        return -1;
    }

    std::unique_ptr<SendPacket> createSendPacket(int size) { return sendPacketPool.getSendPacket(size); }

    void sendPacket(const SendPacket &sendPacket)
    {
        socketInterface.sendTo(reinterpret_cast<char *>(sendPacket.data.data), sendPacket.data.getWriteIndex(),
                               sendPacket.address);
    }

    SSL_CTX *ctx;
    std::optional<EVP_PKEY *> identityPrivateKey;
    std::optional<EVP_PKEY *> identityPublicKey;
    std::vector<Security::CipherSuite> cipherSuitesAvailable;
    std::vector<Security::EphemeralKeyGroup> supportedEphemeralGroups;
    std::unordered_map<Address, P>;
    bool allowInsecureConnections;
    SendPacketPool sendPacketPool;

    SocketInterface socketInterface;
};

class Protocol
{
public:
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
}
