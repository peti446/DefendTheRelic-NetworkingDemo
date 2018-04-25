#ifndef NETWORK_H
#define NETWORK_H


#include <SFML/Network.hpp>

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
        sf::UdpSocket m_udpSocket;
        sf::TcpSocket m_tcpSocket;
        std::string m_registredName;
};

#endif // NETWORK_H
