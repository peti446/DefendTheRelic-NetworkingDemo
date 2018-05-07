#ifndef ENCRYPTEDNETMESSAGE_H
#define ENCRYPTEDNETMESSAGE_H

#include <NetMessage.hpp>


class EncryptedNetMessage : public NetMessage
{
    public:
        EncryptedNetMessage(const sf::Packet& p);
        virtual ~EncryptedNetMessage();

        NetMessage decrypt(const CryptoPP::SecByteBlock& key);
    protected:

    private:
        sf::Packet m_encryptedPacket;
};

#endif // ENCRYPTEDNETMESSAGE_H
