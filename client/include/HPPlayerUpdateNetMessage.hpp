#ifndef HPPLAYERUPDATENETMESSAGE_HPP
#define HPPLAYERUPDATENETMESSAGE_HPP

#include <NetMessage.hpp>


class HPPlayerUpdateNetMessage : public NetMessage
{
    public:
        HPPlayerUpdateNetMessage(std::string whoChanged = "", sf::Int64 hp = 0);
        virtual ~HPPlayerUpdateNetMessage();

        void BuildMessage(sf::Packet& p) final override;
        eNetMessageType getType() const final override;
        sf::Packet BuildPacket() const final override;

        std::string Who;
        sf::Int64 newHp;
    protected:

    private:
};

#endif // HPPLAYERUPDATENETMESSAGE_HPP
