#ifndef BASE64HELPER_H
#define BASE64HELPER_H

#include <Cryptopp/base64.h>
#include <string>

class Base64Helper
{
    public:
        static bool encode(CryptoPP::byte* data, size_t lenght, std::string& outStr);
        static bool decode(const std::string& data, std::string& outStr);
    protected:

    private:
        Base64Helper();
        virtual ~Base64Helper();
};

#endif // BASE64HELPER_H
