#ifndef PLAYERPOSUPDATENETMESSAGE_HPP
#define PLAYERPOSUPDATENETMESSAGE_HPP

#include <NetMessage.hpp>


class PlayerStatusUpdateNetMessage : public NetMessage
{
    public:
        PlayerStatusUpdateNetMessage(std::string s = "", const sf::Vector2f& pos = sf::Vector2f(0,0), sf::Uint16 dir = 0, sf::Uint16 st = 0);
        virtual ~PlayerStatusUpdateNetMessage();

        void BuildMessage(sf::Packet& p) final override;
        eNetMessageType getType() const final override;
        sf::Packet BuildPacket() const final override;

        sf::Vector2f Position;
        sf::Uint16 Direction;
        sf::Uint16 Status;
        std::string Who;
    protected:
    private:
};

#endif // PLAYERPOSUPDATENETMESSAGE_HPP
