#include "EndGameNetMEssage.hpp"

EndGameNetMEssage::EndGameNetMEssage()
{
    //ctor
}

EndGameNetMEssage::~EndGameNetMEssage()
{
    //dtor
}
void EndGameNetMEssage::BuildMessage(sf::Packet& p)
{
    p >> WinnerTeam >> t1_kills >> t2_kills;
}

eNetMessageType EndGameNetMEssage::getType() const
{
    return eNetMessageType::eGameFinishedMessage;
}

sf::Packet EndGameNetMEssage::BuildPacket() const
{
    sf::Packet p;
    //We will never build it so...
    return p;
}

