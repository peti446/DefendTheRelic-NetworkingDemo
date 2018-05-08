#include "GameLobbyUpdateNetMessage.hpp"

GameLobbyUpdateNetMessage::GameLobbyUpdateNetMessage(bool create, std::string player1)
{
    if(create)
    {
        Identifier = "-1create";
        t1_p1 = player1;
        t1_p2 = "NONE";
        t2_p1 = "NONE";
        t2_p2 = "NONE";
    }
}

GameLobbyUpdateNetMessage::~GameLobbyUpdateNetMessage()
{
    //dtor
}

void GameLobbyUpdateNetMessage::BuildMessage(sf::Packet& p)
{
    p >> Identifier >> t1_p1 >> t1_p2 >> t2_p1 >> t2_p2;
}

eNetMessageType GameLobbyUpdateNetMessage::getType() const
{
    return eNetMessageType::eGameLobbyUpdated;
}

sf::Packet GameLobbyUpdateNetMessage::BuildPacket() const
{
    sf::Packet p;
    p << StringHelpers::toString((sf::Uint16)getType()) + "-::-" + Identifier + "-::-" + t1_p1 + "-::-" + t1_p2 + "-::-" + t2_p1 + "-::-" + t2_p2;
    return p;
}
