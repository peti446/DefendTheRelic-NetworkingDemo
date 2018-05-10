#include "PlayerBulletCountUpdateNetMessage.hpp"

PlayerBulletCountUpdateNetMessage::PlayerBulletCountUpdateNetMessage(std::string who, sf::Int64 currentAmount, sf::Int64 curentMaxAmount)
{
    WhoUpdating = who;
    NewAmount = currentAmount;
    NewMaxAmount = curentMaxAmount;
}

PlayerBulletCountUpdateNetMessage::~PlayerBulletCountUpdateNetMessage()
{
    //dtor
}

void PlayerBulletCountUpdateNetMessage::BuildMessage(sf::Packet& p)
{
    p >> WhoUpdating >> NewAmount >> NewMaxAmount;
}

eNetMessageType PlayerBulletCountUpdateNetMessage::getType() const
{
    return eNetMessageType::ePlayerBulletCountUpdate;
}

sf::Packet PlayerBulletCountUpdateNetMessage::BuildPacket() const
{
    sf::Packet p;
    p << StringHelpers::toString((sf::Uint16)getType()) + "-::-" + WhoUpdating + "-::-" + StringHelpers::toString(NewAmount) + "-::-" + StringHelpers::toString(NewMaxAmount);
    return p;
}
