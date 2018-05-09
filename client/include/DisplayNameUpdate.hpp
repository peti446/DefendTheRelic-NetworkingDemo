#ifndef DISPLAYNAMEUPDATE_HPP
#define DISPLAYNAMEUPDATE_HPP

#include <NetMessage.hpp>


class DisplayNameUpdate : public NetMessage
{
    public:
        DisplayNameUpdate(const std::string& s = "");
        virtual ~DisplayNameUpdate();

        void BuildMessage(sf::Packet& p) final override;
        eNetMessageType getType() const final override;
        sf::Packet BuildPacket() const final override;

        std::string DisplayName;
    protected:

    private:
};

#endif // DISPLAYNAMEUPDATE_HPP
