#include "PlayerRespawnNetMessage.hpp"

PlayerRespawnNetMessage::PlayerRespawnNetMessage(std::string who , sf::Vector2f where)
{
    WhoRespawned = who;
    RespawnPos = where;
}

PlayerRespawnNetMessage::~PlayerRespawnNetMessage()
{
    //dtor
}

void PlayerRespawnNetMessage::BuildMessage(sf::Packet& p)
{
    std::string x;
    std::string y;
    p >> WhoRespawned >> x >> y;
    RespawnPos.x = std::stof(x);
    RespawnPos.y = std::stof(y);
}

eNetMessageType PlayerRespawnNetMessage::getType() const
{
    return eNetMessageType::ePlayerRespanwsMessage;
}

sf::Packet PlayerRespawnNetMessage::BuildPacket() const
{
    sf::Packet p;
    p << StringHelpers::toString((sf::Uint16)getType()) + "-::-" + WhoRespawned + "-::-" + StringHelpers::toString(RespawnPos.x) + "-::-" + StringHelpers::toString(RespawnPos.y);
    return p;
}
