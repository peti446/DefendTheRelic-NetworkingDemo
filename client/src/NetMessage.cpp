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
    std::string cipher = AESHelper::encryptCombineIV(key, StringHelpers::convertPacketToString(BuildPacket()));
    Base64Helper::encode((CryptoPP::byte*)cipher.c_str(), cipher.size(), cipher);
    sf::Packet p;
    p << cipher;
    return p;
}
