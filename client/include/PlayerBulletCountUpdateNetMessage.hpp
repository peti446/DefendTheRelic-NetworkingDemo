#ifndef PLAYERBULLETCOUNTUPDATENETMESSAGE_H
#define PLAYERBULLETCOUNTUPDATENETMESSAGE_H

#include <NetMessage.hpp>


class PlayerBulletCountUpdateNetMessage : public NetMessage
{
    public:
        PlayerBulletCountUpdateNetMessage(std::string who = "", sf::Int64 currentAmount = 0, sf::Int64 curentMaxAmount = 0);
        virtual ~PlayerBulletCountUpdateNetMessage();

        void BuildMessage(sf::Packet& p) final override;
        eNetMessageType getType() const final override;
        sf::Packet BuildPacket() const final override;

        std::string WhoUpdating;
        sf::Int64 NewAmount;
        sf::Int64 NewMaxAmount;
    protected:

    private:
};

#endif // PLAYERBULLETCOUNTUPDATENETMESSAGE_H
