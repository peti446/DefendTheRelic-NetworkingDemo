#include "StartGameNetMessage.hpp"
#include "GlobalLobbyScene.hpp"

StartGameNetMessage::StartGameNetMessage(LobbyInfoDisplay& lobby)
{
    Identifier = lobby.getIdentifier();
    t1_p1 = lobby.getTeam1Player1();
    t1_p2 = lobby.getTeam1Player2();
    t2_p1 = lobby.getTeam2Player1();
    t2_p2 = lobby.getTeam2Player2();
}

StartGameNetMessage::~StartGameNetMessage()
{
}

eNetMessageType StartGameNetMessage::getType() const
{
    return eNetMessageType::eGameLobbyStartGame;
}

