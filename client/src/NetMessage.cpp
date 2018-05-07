#include "NetMessage.hpp"
#include <Cryptopp/filters.h>
#include "Base64Helper.hpp"

NetMessage::NetMessage()
{
    //ctor
}

NetMessage::~NetMessage()
{
    //dtor
}

sf::Packet NetMessage::BuildEncryptPacket(const CryptoPP::SecByteBlock& key) const
{
    sf::Packet p = BuildPacket();
    std::string toEncrypt;
    CryptoPP::ArraySource as((CryptoPP::byte*)p.getData(), p.getDataSize(), true, new CryptoPP::StringSink(toEncrypt));
    std::string cipher = AESHelper::encryptCombineIV(key, toEncrypt);
    Base64Helper::encode((CryptoPP::byte*)cipher.c_str(), cipher.size(), cipher);
    p.clear();
    p << cipher;
    return p;
}
