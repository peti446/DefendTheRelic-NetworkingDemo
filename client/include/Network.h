#ifndef NETWORK_H
#define NETWORK_H

#include <SFML/Network.hpp>
#include <Cryptopp/aes.h>
#include <Cryptopp/modes.h>

class Network
{
    public:
        Network();
        virtual ~Network();

        //Connects to the server to set up everything neede.
        //By default it will try to find the server on the local network by using broadcast and the port 13001,
        //but you can pass it an IP(lan or public) and a port to connect to a server directly wich is not limited to be on the lan network.
        bool ConnectToServer(sf::IpAddress IPOfServer = sf::IpAddress::Broadcast, unsigned short port = 13001, size_t amoutOfConnectionAttempts = 3);

    protected:

    private:
        struct ServerDetails
        {
            sf::IpAddress IP;
            sf::Uint16 udp_port;
            sf::Uint16 tcp_port;

            ServerDetails() : IP(sf::IpAddress::None), udp_port(0), tcp_port(0){}
        };

        sf::UdpSocket m_udpSocket;
        sf::TcpSocket m_tcpSocket;
        std::string m_registredName;
        std::string m_displayName;
        ServerDetails m_serverData;
        CryptoPP::SecByteBlock m_aesKey;
};

#endif // NETWORK_H
