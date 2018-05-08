#ifndef CREATEGAMELOBBYNETMESSAGE_H
#define CREATEGAMELOBBYNETMESSAGE_H

#include <NetMessage.hpp>


class CreateGameLobbyNetMessage : public NetMessage
{
    public:
        CreateGameLobbyNetMessage(bool create = false, std::string player1 = "");
        virtual ~CreateGameLobbyNetMessage();

        void BuildMessage(sf::Packet& p) final override;
        eNetMessageType getType() const final override;
        sf::Packet BuildPacket() const final override;

        std::string Identifier;
        std::string t1_p1;
        std::string t1_p2;
        std::string t2_p1;
        std::string t2_p2;
    protected:

    private:
};

#endif // CREATEGAMELOBBYNETMESSAGE_H
