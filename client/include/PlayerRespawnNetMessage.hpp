#ifndef PLAYERRESPAWNNETMESSAGE_HPP
#define PLAYERRESPAWNNETMESSAGE_HPP

#include <NetMessage.hpp>


class PlayerRespawnNetMessage : public NetMessage
{
    public:
        PlayerRespawnNetMessage(std::string who = "", sf::Vector2f where = sf::Vector2f(0,0));
        virtual ~PlayerRespawnNetMessage();

        virtual void BuildMessage(sf::Packet& p) final override;
        virtual eNetMessageType getType() const final override;
        virtual sf::Packet BuildPacket() const final override;

        std::string WhoRespawned;
        sf::Vector2f RespawnPos;
    protected:

    private:
};

#endif // PLAYERRESPAWNNETMESSAGE_HPP
