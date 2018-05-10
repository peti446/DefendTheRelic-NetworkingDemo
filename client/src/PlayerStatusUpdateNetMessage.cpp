#include "PlayerStatusUpdateNetMessage.hpp"

PlayerStatusUpdateNetMessage::PlayerStatusUpdateNetMessage(std::string s, const sf::Vector2f& pos, sf::Uint16 dir, sf::Uint16 st)
{
    Who = s;
    Position = pos;
    Direction = dir;
    Status = st;
}

PlayerStatusUpdateNetMessage::~PlayerStatusUpdateNetMessage()
{
    //dtor
}

void PlayerStatusUpdateNetMessage::BuildMessage(sf::Packet& p)
{
    p  >> Who >> Position.x >> Position.y >> Direction >> Status;
}

eNetMessageType PlayerStatusUpdateNetMessage::getType() const
{
    return eNetMessageType::eInGamePlayerPosUpdate;
}

sf::Packet PlayerStatusUpdateNetMessage::BuildPacket() const
{
    sf::Packet p;
    p << StringHelpers::toString((sf::Uint16)getType()) + "-::-" + Who + "-::-" + StringHelpers::toString(Position.x) + "-::-" + StringHelpers::toString(Position.y) + "-::-" +
        StringHelpers::toString((sf::Uint16)Direction) + "-::-" +  StringHelpers::toString((sf::Uint16)Status);
    return p;
}
