#ifndef AESHELPER_H
#define AESHELPER_H

#include <string>
#include <Cryptopp/aes.h>

class AESHelper
{
    public:

        static CryptoPP::SecByteBlock generateKey(const size_t& Keysize);
        static std::string encrypt(const CryptoPP::SecByteBlock& key, const std::string& msgToEncrypt, CryptoPP::SecByteBlock& outIV);
        static std::string decrypt(const CryptoPP::SecByteBlock& key, const std::string& msgToDecrypt, const CryptoPP::SecByteBlock& IV);
        static std::string decryptCombinedIV(const CryptoPP::SecByteBlock& key, const std::string& combinedMsg);
    protected:

    private:
        AESHelper();
        virtual ~AESHelper();
};

#endif // AESHELPER_H
