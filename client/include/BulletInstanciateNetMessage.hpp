#ifndef BULLETINSTANCIATENETMESSAGE_HPP
#define BULLETINSTANCIATENETMESSAGE_HPP

#include <NetMessage.hpp>


class BulletInstanciateNetMessage : public NetMessage
{
    public:
        BulletInstanciateNetMessage(std::string who = "", sf::Vector2f startPos = sf::Vector2f(0.0f,0.0f), sf::Uint16 dir = 0.0f, float speed = 0.0f);
        virtual ~BulletInstanciateNetMessage();

        void BuildMessage(sf::Packet& p) final override;
        eNetMessageType getType() const final override;
        sf::Packet BuildPacket() const final override;

        std::string WhoShoot;
        sf::Vector2f StartPosition;
        sf::Uint16 Direction;
        float BulletSpeed;
    protected:

    private:
};

#endif // BULLETINSTANCIATENETMESSAGE_HPP
