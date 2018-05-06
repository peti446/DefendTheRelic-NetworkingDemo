#include "RSAHelper.hpp"
#include <Cryptopp/osrng.h>

RSAHelper::RSAHelper()
{
    //ctor
}

RSAHelper::~RSAHelper()
{
    //dtor
}

CryptoPP::RSA::PublicKey RSAHelper::getPublicKeyFromPemString(const std::string& keyString)
{
    CryptoPP::RSA::PublicKey pk;
    CryptoPP::StringSource ss(keyString, true);
    CryptoPP::PEM_Load(ss ,pk);
    return pk;
}

std::string RSAHelper::encrypt(const CryptoPP::RSA::PublicKey& pub_key, const std::string& plainText)
{
    CryptoPP::AutoSeededRandomPool rnd;
    std::string out;

    CryptoPP::RSAES<CryptoPP::PKCS1v15>::Encryptor RSAES_OAPE_SHA_Encryptor(pub_key);
    CryptoPP::StringSource ess(plainText, true, new CryptoPP::PK_EncryptorFilter(rnd, RSAES_OAPE_SHA_Encryptor, new CryptoPP::StringSink(out)));
    return out;
}

std::string RSAHelper::encrypt(const CryptoPP::RSA::PublicKey& pub_key, const CryptoPP::byte* data, const size_t size)
{
    CryptoPP::AutoSeededRandomPool rnd;
    std::string out;
    CryptoPP::RSAES<CryptoPP::PKCS1v15>::Encryptor RSAES_OAPE_SHA_Encryptor(pub_key);
    //CryptoPP::ArraySource as(data, size, true, new CryptoPP::PK_EncryptorFilter(rnd, RSAES_OAPE_SHA_Encryptor, new CryptoPP::StringSink(out)));
    CryptoPP::ArraySource as(data, size, true, new CryptoPP::StringSink(out));
    return RSAHelper::encrypt(pub_key, out);
}
