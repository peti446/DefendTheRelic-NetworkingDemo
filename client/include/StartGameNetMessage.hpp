#ifndef STARTGAMENETMESSAGE_HPP
#define STARTGAMENETMESSAGE_HPP

#include <GameLobbyUpdateNetMessage.hpp>
class LobbyInfoDisplay;

class StartGameNetMessage : public GameLobbyUpdateNetMessage
{
    public:
        StartGameNetMessage(LobbyInfoDisplay& lobby);
        StartGameNetMessage() = default;
        virtual ~StartGameNetMessage();

        eNetMessageType getType() const final override;

    protected:

    private:
};

#endif // STARTGAMENETMESSAGE_HPP
