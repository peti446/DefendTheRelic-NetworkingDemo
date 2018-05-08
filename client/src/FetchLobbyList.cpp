#include "FetchLobbyList.hpp"

FetchLobbyList::FetchLobbyList()
{
    //ctor
}

FetchLobbyList::~FetchLobbyList()
{
    //dtor
}

void FetchLobbyList::BuildMessage(sf::Packet& p)
{
}

eNetMessageType FetchLobbyList::getType() const
{
    return eNetMessageType::eGetAllGameLobbyInfo;
}

sf::Packet FetchLobbyList::BuildPacket() const
{
    sf::Packet p;
    p << StringHelpers::toString((sf::Uint16)getType()) + "-::-GetAll";
    return p;
}

