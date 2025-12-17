#pragma once
#include <memory>
#include <vector>
#include <openssl/evp.h>
#include <openssl/types.h>

namespace SUP
{
namespace Security
{
enum class CipherSuite
{
    TLS_AES_128_GCM_SHA256 = 0x1301,
    TLS_AES_256_GCM_SHA384 = 0x1302,
    TLS_CHACHA20_POLY1305_SHA256 = 0x1303
};

constexpr std::array<CipherSuite, 3> AllCipherSuites = {
    CipherSuite::TLS_AES_128_GCM_SHA256,
    CipherSuite::TLS_AES_256_GCM_SHA384,
    CipherSuite::TLS_CHACHA20_POLY1305_SHA256
};

constexpr std::optional<CipherSuite> idToCipherSuite(int id)
{
    const auto cs = static_cast<CipherSuite>(id);

    switch (cs)
    {
        case CipherSuite::TLS_AES_128_GCM_SHA256:
        case CipherSuite::TLS_AES_256_GCM_SHA384:
        case CipherSuite::TLS_CHACHA20_POLY1305_SHA256:
            return cs;
        default:
            return std::nullopt;
    }
}

#define CIPHER_SUITES_CASE_TO_NAME(name) \
case CipherSuite::name: return #name;

constexpr std::string_view cipherSuiteToOpenSSLName(const CipherSuite cs)
{
    switch (cs)
    {
        CIPHER_SUITES_CASE_TO_NAME(TLS_AES_128_GCM_SHA256)
        CIPHER_SUITES_CASE_TO_NAME(TLS_AES_256_GCM_SHA384)
        CIPHER_SUITES_CASE_TO_NAME(TLS_CHACHA20_POLY1305_SHA256)
    }
    __builtin_unreachable();
}

enum class EphemeralKeyGroup
{
    X448 = 30,
    x25519 = 29,
    secp384r1 = 24,
    secp256r1 = 23
};

constexpr int MAX_EPHEMERAL_KEY_VALUE_LENGTH = 1024 / sizeof(uint8_t);

constexpr int ephemeralKeyGroupToOpenSSLNid(EphemeralKeyGroup keyType)
{
    switch (keyType)
    {
        case EphemeralKeyGroup::X448: return NID_X448;
        case EphemeralKeyGroup::x25519: return NID_X25519;
        case EphemeralKeyGroup::secp384r1: return NID_secp384r1;
        case EphemeralKeyGroup::secp256r1: return NID_X9_62_prime256v1;
    }
    __builtin_unreachable();
}

#define INT_TO_EPHEMERAL_KEY_GROUP_CASE(KeyGroup) \
    case static_cast<int>(EphemeralKeyGroup::KeyGroup): return EphemeralKeyGroup::KeyGroup;

constexpr std::optional<EphemeralKeyGroup> getEphemeralKeyGroup(const int id)
{
    switch (id)
    {
        INT_TO_EPHEMERAL_KEY_GROUP_CASE(X448)
        INT_TO_EPHEMERAL_KEY_GROUP_CASE(x25519)
        INT_TO_EPHEMERAL_KEY_GROUP_CASE(secp384r1)
        INT_TO_EPHEMERAL_KEY_GROUP_CASE(secp256r1)
        default:
            return std::nullopt;
    }
}
}
}
