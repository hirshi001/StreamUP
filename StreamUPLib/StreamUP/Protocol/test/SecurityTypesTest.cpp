#include <random>
#include <boost/test/unit_test.hpp>
#include <openssl/ssl.h>

#include "Protocol/AuthEncryptionTypes.h"

#include "../../Platform/include/Platform/Address.h"

using namespace SUP;

bool openSSLHasCipherSuite(const std::string_view& name)
{
    STACK_OF(SSL_CIPHER)* list = SSL_CTX_get_ciphers(SSL_CTX_new(TLS_method()));
    for (int i = 0; i < sk_SSL_CIPHER_num(list); ++i)
    {
        const SSL_CIPHER* c = sk_SSL_CIPHER_value(list, i);
        if (name == SSL_CIPHER_get_name(c))
            return true;
    }
    return false;
}

BOOST_AUTO_TEST_CASE(SecurityTypesTests)
{
    auto ciphers = Security::AllCipherSuites;
    for (auto cipher : ciphers)
    {
        const std::string_view& name = Security::cipherSuiteToOpenSSLName(cipher);
        std::cout << name << std::endl;

        BOOST_TEST_MESSAGE("Checking: " << name);
        BOOST_CHECK(openSSLHasCipherSuite(name));
    }
}