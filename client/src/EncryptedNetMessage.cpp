#include "EncryptedNetMessage.hpp"

EncryptedNetMessage::EncryptedNetMessage(const sf::Packet& p)
{
    m_encryptedPacket = p;
}

EncryptedNetMessage::~EncryptedNetMessage()
{
    //dtor
}

NetMessage EncryptedNetMessage::decrypt(const CryptoPP::SecByteBlock& key)
{

}

