#include "EmptyNetMessage.hpp"

EmptyNetMessage::EmptyNetMessage()
{
    //ctor
}

EmptyNetMessage::~EmptyNetMessage()
{
    //dtor
}

void EmptyNetMessage::BuildMessage(sf::Packet& p)
{

}

eNetMessageType EmptyNetMessage::getType() const
{
    return eNetMessageType::eEmpty;
}

sf::Packet EmptyNetMessage::BuildPacket() const
{
    return sf::Packet();
}
