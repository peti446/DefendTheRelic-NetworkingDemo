#ifndef FETCHLOBBYLIST_HPP
#define FETCHLOBBYLIST_HPP

#include <NetMessage.hpp>


class FetchLobbyList : public NetMessage
{
    public:
        FetchLobbyList();
        virtual ~FetchLobbyList();

        void BuildMessage(sf::Packet& p) final override;
        eNetMessageType getType() const final override;
        sf::Packet BuildPacket() const final override;
    protected:

    private:
};

#endif // FETCHLOBBYLIST_HPP
