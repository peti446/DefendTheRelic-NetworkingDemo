#ifndef NETMESSAGE_H
#define NETMESSAGE_H
#include <SFML/Network.hpp>
#include "AESHelper.hpp"
#include "Logger.hpp"
#include "StringHelpers.hpp"


enum class eNetMessageType
{
    eEmpty,
    eEncrypted,
    eDisplayNameUpdate,
    eShoot,
    eMove,
    eDie,
    eRespawned
};

class NetMessage
{
    public:
        NetMessage();
        virtual ~NetMessage();

        virtual void BuildMessage(sf::Packet p) = 0;
        virtual eNetMessageType getType() const = 0;
        virtual sf::Packet BuildPacket() const = 0;
        sf::Packet BuildEncryptPacket(const CryptoPP::SecByteBlock& key) const;
    protected:
    private:
};

#endif // NETMESSAGE_H
