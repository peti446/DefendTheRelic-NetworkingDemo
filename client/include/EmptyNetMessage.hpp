#ifndef EMPTYNETMESSAGE_H
#define EMPTYNETMESSAGE_H

#include <NetMessage.hpp>


class EmptyNetMessage : public NetMessage
{
    public:
        EmptyNetMessage();
        virtual ~EmptyNetMessage();


        void BuildMessage(sf::Packet& p) final override;
        eNetMessageType getType() const final override;
        sf::Packet BuildPacket() const final override;
    protected:

    private:
};

#endif // EMPTYNETMESSAGE_H
