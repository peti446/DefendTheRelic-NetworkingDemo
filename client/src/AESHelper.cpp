#include "AESHelper.hpp"
#include <Cryptopp/osrng.h>
#include <Cryptopp/modes.h>

AESHelper::AESHelper()
{
}

AESHelper::~AESHelper()
{
}

CryptoPP::SecByteBlock AESHelper::generateKey(const size_t& Keysize)
{
    CryptoPP::AutoSeededRandomPool rnd;
    CryptoPP::SecByteBlock Key(0x00, Keysize);
    rnd.GenerateBlock(Key, Key.size());
    return Key;
}

std::string AESHelper::encrypt(const CryptoPP::SecByteBlock& key, const std::string& msgToEncrypt, CryptoPP::SecByteBlock& outIV)
{
    CryptoPP::AutoSeededRandomPool rnd;
    outIV = CryptoPP::SecByteBlock(16);
    rnd.GenerateBlock(outIV, outIV.size());
    CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption encryptor(key, key.size(), outIV);
    std::string out;
    CryptoPP::StringSource ss( msgToEncrypt, true,
        new CryptoPP::StreamTransformationFilter( encryptor,
            new CryptoPP::StringSink( out )
        ) // StreamTransformationFilter
    ); // StringSource
    return out;
}

std::string AESHelper::encryptCombineIV(const CryptoPP::SecByteBlock& key, const std::string& msgToEncrypt)
{
    CryptoPP::SecByteBlock iv;
    std::string encrypted = AESHelper::encrypt(key, msgToEncrypt, iv);
    std::string ivString;
    CryptoPP::ArraySource as(iv, iv.size(), true, new CryptoPP::StringSink(ivString));
    return ivString + encrypted;
}

std::string AESHelper::decrypt(const CryptoPP::SecByteBlock& key, const std::string& msgToDecrypt, const CryptoPP::SecByteBlock& IV)
{
    CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption decryptor(key, key.size(), IV);
    std::string out;
    CryptoPP::StringSource ss( msgToDecrypt, true,
        new CryptoPP::StreamTransformationFilter( decryptor,
            new CryptoPP::StringSink( out )
        ) // StreamTransformationFilter
    ); // StringSource
    return out;
}

std::string AESHelper::decryptCombinedIV(const CryptoPP::SecByteBlock& key, const std::string& combinedMsg)
{
    CryptoPP::SecByteBlock iv(16);
    CryptoPP::ArraySink av(iv, iv.size());
    CryptoPP::StringSource stringKey(combinedMsg, false, new CryptoPP::Redirector(av));
    stringKey.Pump(16);
    std::string chipherText;
    stringKey.Detach(new CryptoPP::StringSink(chipherText));
    stringKey.PumpAll();

    return AESHelper::decrypt(key, chipherText, iv);
}
