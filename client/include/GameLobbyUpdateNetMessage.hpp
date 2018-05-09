#ifndef CREATEGAMELOBBYNETMESSAGE_H
#define CREATEGAMELOBBYNETMESSAGE_H

#include <NetMessage.hpp>


class GameLobbyUpdateNetMessage : public NetMessage
{
    public:
        GameLobbyUpdateNetMessage(bool create = false, std::string player1 = "");
        virtual ~GameLobbyUpdateNetMessage();

        void BuildMessage(sf::Packet& p) final override;
        virtual eNetMessageType getType() const override;
        sf::Packet BuildPacket() const final override;

        std::string Identifier;
        std::string t1_p1;
        std::string t1_p2;
        std::string t2_p1;
        std::string t2_p2;
        std::string Status;
    protected:

    private:
};

#endif // CREATEGAMELOBBYNETMESSAGE_H
