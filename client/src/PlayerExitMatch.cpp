#include "PlayerExitMatch.hpp"

PlayerExitMatch::PlayerExitMatch(std::string who)
{
    WhoDisconnected = who;
}

PlayerExitMatch::~PlayerExitMatch()
{
    //dtor
}

void PlayerExitMatch::BuildMessage(sf::Packet& p)
{
    p >> WhoDisconnected;
}

eNetMessageType PlayerExitMatch::getType() const
{
    return eNetMessageType::ePlayerQuitMessage;
}

sf::Packet PlayerExitMatch::BuildPacket() const
{
    sf::Packet p;
    p << StringHelpers::toString((sf::Uint16)getType()) + "-::-" + WhoDisconnected;
    return p;
}
