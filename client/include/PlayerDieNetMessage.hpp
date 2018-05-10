#ifndef PLAYERDIENETMESSAGE_HPP
#define PLAYERDIENETMESSAGE_HPP

#include <NetMessage.hpp>


class PlayerDieNetMessage : public NetMessage
{
    public:
        PlayerDieNetMessage(std::string diedChar = "", std::string killer = "");
        virtual ~PlayerDieNetMessage();

        virtual void BuildMessage(sf::Packet& p) final override;
        virtual eNetMessageType getType() const final override;
        virtual sf::Packet BuildPacket() const final override;

        std::string WhoDied;
        std::string ByWho;
    protected:

    private:
};

#endif // PLAYERDIENETMESSAGE_HPP
