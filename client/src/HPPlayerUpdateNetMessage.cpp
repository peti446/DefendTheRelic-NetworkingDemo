#include "HPPlayerUpdateNetMessage.hpp"

HPPlayerUpdateNetMessage::HPPlayerUpdateNetMessage(std::string whoChanged, sf::Int64 hp)
{
   Who = whoChanged;
   newHp = hp;
}

HPPlayerUpdateNetMessage::~HPPlayerUpdateNetMessage()
{
    //dtor
}
void HPPlayerUpdateNetMessage::BuildMessage(sf::Packet& p)
{
    p >> Who >> newHp;
}

eNetMessageType HPPlayerUpdateNetMessage::getType() const
{
    return eNetMessageType::eHpPlayerUpdate;
}

sf::Packet HPPlayerUpdateNetMessage::BuildPacket() const
{
    sf::Packet p;
    p << StringHelpers::toString((sf::Uint16)getType()) + "-::-" + Who + "-::-" + StringHelpers::toString(newHp);
    return p;
}

