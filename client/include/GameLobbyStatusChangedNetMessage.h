#ifndef LOBBYSTATUSCHANGEDNETMESSAGE_H
#define LOBBYSTATUSCHANGEDNETMESSAGE_H

#include <NetMessage.hpp>


class GameLobbyStatusChangedNetMessage : public NetMessage
{
    public:
        enum class UpdateMsg
        {
            eJoin,
            eSwitch,
            eExit
        };
        GameLobbyStatusChangedNetMessage(std::string identifier_, UpdateMsg update_);
        virtual ~GameLobbyStatusChangedNetMessage();

        void BuildMessage(sf::Packet& p) final override;
        eNetMessageType getType() const final override;
        sf::Packet BuildPacket() const final override;

        std::string Identifier;
        UpdateMsg Update;
    protected:

    private:
};

#endif // LOBBYSTATUSCHANGEDNETMESSAGE_H
