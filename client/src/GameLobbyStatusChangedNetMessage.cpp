#include "GameLobbyStatusChangedNetMessage.h"

GameLobbyStatusChangedNetMessage::GameLobbyStatusChangedNetMessage(std::string identifier_, UpdateMsg update_)
{
    Identifier = identifier_;
    Update = update_;
}

GameLobbyStatusChangedNetMessage::~GameLobbyStatusChangedNetMessage()
{
    //dtor
}

void GameLobbyStatusChangedNetMessage::BuildMessage(sf::Packet& p)
{
    sf::Uint16 type;
    p >> Identifier >> type;
    Update = (UpdateMsg)type;
}

eNetMessageType GameLobbyStatusChangedNetMessage::getType() const
{
    return eNetMessageType::eGameLobbyStatusUpdated;
}

sf::Packet GameLobbyStatusChangedNetMessage::BuildPacket() const
{
    sf::Packet p;
    std::string s = "join";
    if(Update == UpdateMsg::eExit)
    {
        s = "exit";
    } else if(Update == UpdateMsg::eSwitch)
    {
        s = "switch";
    }
    p << StringHelpers::toString((sf::Uint16)getType()) + "-::-" + Identifier + "-::-" + s;
    return p;
}
