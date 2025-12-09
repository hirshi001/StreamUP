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
    TLS_AES_128_GCM_SHA256,
    TLS_AES_256_GCM_SHA384,
    TLS_CHACHA20_POLY1305_SHA256
};

constexpr std::array<CipherSuite, 3> AllCipherSuites = {
    CipherSuite::TLS_AES_128_GCM_SHA256,
    CipherSuite::TLS_AES_256_GCM_SHA384,
    CipherSuite::TLS_CHACHA20_POLY1305_SHA256
};

#define CIPHER_SUITES_CASE_TO_NAME(name) \
case CipherSuite::name: return #name;

constexpr std::string_view cipherSuiteToOpenSSLName(const CipherSuite cs)
{
    switch (cs) {
        CIPHER_SUITES_CASE_TO_NAME(TLS_AES_128_GCM_SHA256)
        CIPHER_SUITES_CASE_TO_NAME(TLS_AES_256_GCM_SHA384)
        CIPHER_SUITES_CASE_TO_NAME(TLS_CHACHA20_POLY1305_SHA256)
    }
    __builtin_unreachable();
}

}
}
