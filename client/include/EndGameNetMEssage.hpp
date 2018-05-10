#ifndef ENDGAMENETMESSAGE_HPP
#define ENDGAMENETMESSAGE_HPP

#include <NetMessage.hpp>


class EndGameNetMEssage : public NetMessage
{
    public:
        EndGameNetMEssage();
        virtual ~EndGameNetMEssage();

        void BuildMessage(sf::Packet& p) final override;
        eNetMessageType getType() const final override;
        sf::Packet BuildPacket() const final override;

        std::string WinnerTeam;
        sf::Uint8 t1_kills;
        sf::Uint8 t2_kills;
    protected:

    private:
};

#endif // ENDGAMENETMESSAGE_HPP
