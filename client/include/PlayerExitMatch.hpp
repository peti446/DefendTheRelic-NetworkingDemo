#ifndef PLAYEREXITMATCH_HPP
#define PLAYEREXITMATCH_HPP

#include <NetMessage.hpp>


class PlayerExitMatch : public NetMessage
{
    public:
        PlayerExitMatch(std::string who = "");
        virtual ~PlayerExitMatch();

        void BuildMessage(sf::Packet& p) final override;
        eNetMessageType getType() const final override;
        sf::Packet BuildPacket() const final override;

        std::string WhoDisconnected;
    protected:

    private:
};

#endif // PLAYEREXITMATCH_HPP
