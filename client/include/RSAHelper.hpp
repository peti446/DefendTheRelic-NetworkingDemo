#ifndef RSAHELPER_H
#define RSAHELPER_H

#include <Cryptopp/rsa.h>
#include <Cryptopp/pem.h>

class RSAHelper
{
    public:
        static CryptoPP::RSA::PublicKey getPublicKeyFromPemString(const std::string& keyString);
        static std::string encrypt(const CryptoPP::RSA::PublicKey& pub_key, const std::string& plainText);
        static std::string encrypt(const CryptoPP::RSA::PublicKey& pub_key, const CryptoPP::byte* data, const size_t size);
    protected:

    private:
        RSAHelper();
        virtual ~RSAHelper();
};

#endif // RSAHELPER_H
