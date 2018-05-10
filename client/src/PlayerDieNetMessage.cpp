#include "PlayerDieNetMessage.hpp"

PlayerDieNetMessage::PlayerDieNetMessage(std::string diedChar, std::string killer)
{
    WhoDied = diedChar;
    ByWho = killer;
}

PlayerDieNetMessage::~PlayerDieNetMessage()
{
    //dtor
}

void PlayerDieNetMessage::BuildMessage(sf::Packet& p)
{
    p >> WhoDied >> ByWho;
}

eNetMessageType PlayerDieNetMessage::getType() const
{
    return eNetMessageType::ePlayerDiesMessage;
}

sf::Packet PlayerDieNetMessage::BuildPacket() const
{
    sf::Packet p;
    p << StringHelpers::toString((sf::Uint16)getType()) + "-::-" + WhoDied + "-::-" + ByWho;
    return p;
}
