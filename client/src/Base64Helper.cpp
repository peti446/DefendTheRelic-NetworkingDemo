#include "Base64Helper.hpp"

Base64Helper::Base64Helper()
{
    //ctor
}

Base64Helper::~Base64Helper()
{
    //dtor
}

bool Base64Helper::encode(CryptoPP::byte* data, size_t lenght, std::string& outStr)
{
    CryptoPP::Base64Encoder encoder;
    encoder.Put(data, lenght);
    encoder.MessageEnd();

    CryptoPP::word64 endoced_size = encoder.MaxRetrievable();
    if(endoced_size)
    {
        outStr.clear();
        outStr.resize(endoced_size);
        encoder.Get((CryptoPP::byte*)&outStr[0], outStr.size());
        return true;
    }
    return false;
}

bool Base64Helper::decode(const std::string& data, std::string& outStr)
{
    CryptoPP::Base64Decoder decoder;
    decoder.Put((CryptoPP::byte*)data.data(), data.size() );
    decoder.MessageEnd();

    CryptoPP::word64 sizeD = decoder.MaxRetrievable();
    if(sizeD && sizeD <= SIZE_MAX)
    {
        outStr.resize(sizeD);
        decoder.Get((CryptoPP::byte*)&outStr[0], outStr.size());
        return true;
    }
    return false;
}
