#include "DisplayNameUpdate.hpp"
#include "StringHelpers.hpp"

DisplayNameUpdate::DisplayNameUpdate(const std::string& s)
{
    DisplayName = s;
}

DisplayNameUpdate::~DisplayNameUpdate()
{
    //dtor
}

void DisplayNameUpdate::BuildMessage(sf::Packet p)
{
    p >> DisplayName;
}

eNetMessageType DisplayNameUpdate::getType() const
{
    return eNetMessageType::eDisplayNameUpdate;
}

sf::Packet DisplayNameUpdate::BuildPacket() const
{
    sf::Packet p;
    p << StringHelpers::toString((sf::Uint16)getType()) << "-::-" << DisplayName;
    return p;
}
