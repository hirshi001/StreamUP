#pragma once

#include "Protocol/AuthEncryptionTypes.h"
#include "Protocol/connection/Connection.h"
#include "Protocol/connection/ConnectionIdHandler.h"
#include "Protocol/connection/ConnectionManager.h"
#include "Protocol/packet/Packet.h"
#include "Protocol/packet/SendPacket.h"
#include "Protocol/packet/SendPacketPool.h"

#include "Platform/SocketInterface.h"
#include "Data/buffer/ReadBufferWrapper.h"

#include <cassert>
#include <expected>
#include <memory>
#include <openssl/ssl.h>
#include <openssl/types.h>

namespace SUP
{
using SSL_CTX_ptr = std::unique_ptr<SSL_CTX, decltype(&SSL_CTX_free)>;

struct EVP_PKEY_handle
{
    EVP_PKEY *p;

    explicit EVP_PKEY_handle(EVP_PKEY *p) : p(p) {}

    ~EVP_PKEY_handle()
    {
        if (p)
            EVP_PKEY_free(p);
    }

    [[nodiscard]] EVP_PKEY *get() const noexcept { return p; }

    EVP_PKEY_handle(EVP_PKEY_handle &&) = delete;

    EVP_PKEY_handle(EVP_PKEY_handle const &) = delete;
};

using EVP_PKEY_ptr = std::shared_ptr<EVP_PKEY_handle>;

enum class SUPVersions
{
    V1 = 1
};


class Protocol
{
    static constexpr int MAX_NUMBER_NEGOTIABLES = 16;

public:
    Protocol(Protocol &) = delete;

    Protocol(Protocol &&) = delete;

    Protocol &operator=(Protocol &) = delete;

    Protocol &operator=(Protocol &&) = delete;

    enum class CreateProtocolError
    {
        FAILED_TO_CREATE_SSL_CTX,
        MISSING_PRIVATE_KEY,
        MISSING_PUBLIC_KEY,
        MISSING_CIPHER_SUITES,
        MISSING_EPHEMERAL_GROUPS
    };

    struct Config final
    {
        std::vector<Security::CipherSuite> cipherSuites;
        std::vector<Security::EphemeralKeyGroup> supportedEphemeralGroups;
        EVP_PKEY_ptr privateKey = nullptr;
        EVP_PKEY_ptr publicKey = nullptr;
        bool allowInsecureConnections;
    };

    static std::expected<std::unique_ptr<Protocol>, CreateProtocolError> createProtocol(const Protocol::Config &config)
    {
        if (!config.allowInsecureConnections)
        {
            if (config.cipherSuites.empty())
                return std::unexpected(CreateProtocolError::MISSING_CIPHER_SUITES);
            if (config.supportedEphemeralGroups.empty())
                return std::unexpected(CreateProtocolError::MISSING_EPHEMERAL_GROUPS);
        }

        if (config.privateKey || config.publicKey) // If one is provided, both must be provided
        {
            if (!config.privateKey)
                return std::unexpected(CreateProtocolError::MISSING_PRIVATE_KEY);
            if (!config.publicKey)
                return std::unexpected(CreateProtocolError::MISSING_PUBLIC_KEY);
        }

        SSL_CTX_ptr ctx{SSL_CTX_new(TLS_method()), SSL_CTX_free};
        if (!ctx)
            return std::unexpected(CreateProtocolError::FAILED_TO_CREATE_SSL_CTX);

        return std::unique_ptr<Protocol>(new Protocol(std::move(ctx), config.cipherSuites,
                                                      config.supportedEphemeralGroups, config.privateKey,
                                                      config.publicKey, false));
    }

private:
    explicit Protocol(SSL_CTX_ptr ctx,
                      const std::vector<Security::CipherSuite> &cipherSuitesAvailable,
                      const std::vector<Security::EphemeralKeyGroup> &supportedEphemeralGroups,
                      const EVP_PKEY_ptr &privateKey,
                      const EVP_PKEY_ptr &publicKey,
                      const bool allowInsecureConnections = false) : ctx(std::move(ctx)),
                                                                     allowInsecureConnections(allowInsecureConnections),
                                                                     sendPacketPool(32),
                                                                     connectionManager()
    {
        assert(ctx != nullptr);
        if (!allowInsecureConnections)
        {
            assert(!cipherSuitesAvailable.empty());
            assert(!supportedEphemeralGroups.empty());
        }

        if (privateKey || publicKey)
        {
            assert(privateKey);
            assert(publicKey);
        }

        this->ctx = std::move(ctx);
        this->supportedEphemeralGroups = supportedEphemeralGroups;
        this->cipherSuitesAvailable = cipherSuitesAvailable;
        this->identityPrivateKey = privateKey;
        this->identityPublicKey = publicKey;
    }

public:

    ~Protocol() = default;


    /**
     * Returns the corresponding connection for the packet
     * @param packet
     */
    Connection* getConnectionForPacket(const std::unique_ptr<Packet> &packet)
    {
        BufferUtil::ReadBufferWrapper reader(packet->buffer.data(), packet->buffer.size());
        Connection::ConnectionId connectionId;
        for (int i = 0; i < Connection::CONNECTION_ID_SIZE; i++)
        {
            connectionId[i] = reader.read<uint8_t>();
        }

        // TODO: Make sure multiple connection ids per packet are handled properly
        Connection* connection = connectionManager.getConnection(connectionId);
        if (!connection) // TODO: Check that the packet follows the format for creating new connections, otherwise we are creating a new connection object for nothing.
            connection = connectionManager.addNewConnection();
        return connection;
    }

    /**
     * Used to create a connection that can connect to another server
     * @return
     */
    Connection* createNewConnection()
    {
        return connectionManager.addNewConnection();
    }


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

    SSL_CTX_ptr ctx;
    EVP_PKEY_ptr identityPrivateKey;
    EVP_PKEY_ptr identityPublicKey;
    std::vector<Security::CipherSuite> cipherSuitesAvailable;
    std::vector<Security::EphemeralKeyGroup> supportedEphemeralGroups;
    bool allowInsecureConnections;
    SendPacketPool sendPacketPool;

    SocketInterface socketInterface;
    ConnectionManager connectionManager;
};
}
