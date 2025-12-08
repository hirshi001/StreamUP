#pragma once
#include <memory>
#include <vector>
#include <openssl/evp.h>
#include <openssl/types.h>

namespace SUP
{
namespace Security
{

enum class CipherSuites
{
    TLS_AES_128_GCM_SHA256 = 0x1301,
    TLS_AES_256_GCM_SHA384 = 0x1302,
    TLS_CHACHA20_POLY1305_SHA256 = 0x1303,
    TLS_AES_128_CCM_SHA256 = 0x1304,
    TLS_AES_128_CCM_8_SHA256 = 0x1305,
};

enum class EphemeralKeyType
{
    X25519 = 0x01,
    P_256_ECDH = 0x02,
    P_384_ECDH = 0x03,
    FFDHE = 0x04,
};

// TODO: Identity Authentication
enum class IdentityKeyType
{
    RSA = 0x01,
    P_256_ECDSA = 0x02,
    P_384_ECDSA = 0x03,
    Ed25519 = 0x04,
};

std::optional<IdentityKeyType> OpenSSLToIdentityKeyType(EVP_PKEY *key)
{
    switch (EVP_PKEY_id(key))
    {
        case EVP_PKEY_RSA:
            return IdentityKeyType::RSA;
        case EVP_PKEY_ED25519:
            return IdentityKeyType::Ed25519;
        case EVP_PKEY_EC:
            // TODO:
            return std::nullopt;
        default:
            return std::nullopt;
    }
}

void SerializePublicIdentityKey(EVP_PKEY *key, IdentityKeyType type, std::vector<uint8_t> &out)
{
    const int len = i2d_PublicKey(key, nullptr);
    std::vector<uint8_t> data(len);
    unsigned char* ptr = data.data();
    i2d_PublicKey(key, &ptr);
}


struct EVPKeyDeleter
{
    void operator()(EVP_PKEY *p) const { EVP_PKEY_free(p); }
};

using EVPKeyPtr = std::unique_ptr<EVP_PKEY, EVPKeyDeleter>;

struct KeyPair
{
    EVPKeyPtr key;
    IdentityKeyType type;
};
// --------

}
}
