#include "BulletInstanciateNetMessage.hpp"

BulletInstanciateNetMessage::BulletInstanciateNetMessage(std::string who, sf::Vector2f startPos, sf::Uint16 dir, float speed)
{
    WhoShoot = who;
    BulletSpeed = speed;
    StartPosition = startPos;
    Direction = dir;
}

BulletInstanciateNetMessage::~BulletInstanciateNetMessage()
{
    //dtor
}

void BulletInstanciateNetMessage::BuildMessage(sf::Packet& p)
{
    std::string s;
    std::string x;
    std::string y;
    p >> WhoShoot >> x >> y >> Direction >> s ;
    BulletSpeed = std::stof(s);
    StartPosition.x = std::stof(x);
    StartPosition.y = std::stof(y);
}

eNetMessageType BulletInstanciateNetMessage::getType() const
{
    return eNetMessageType::eBulletInstanciateMessage;
}

sf::Packet BulletInstanciateNetMessage::BuildPacket() const
{
    sf::Packet p;
    p << StringHelpers::toString((sf::Uint16)getType()) + "-::-" + WhoShoot + "-::-"  +  StringHelpers::toString(StartPosition.x) + "-::-" + StringHelpers::toString(StartPosition.y) + "-::-"
    + StringHelpers::toString(Direction) + "-::-" + StringHelpers::toString(BulletSpeed);
    return p;
}
