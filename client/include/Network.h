#ifndef NETWORK_H
#define NETWORK_H

#include <SFML/Network.hpp>
#include <thread>
#include "AESHelper.hpp"
#include "ConcurrentQueue.hpp"
#include "NetMessage.hpp"

class DisplayNameUpdate;

class Network
{
    public:
        Network();
        virtual ~Network();

        //Connects to the server to set up everything neede.
        //By default it will try to find the server on the local network by using broadcast and the port 13001,
        //but you can pass it an IP(lan or public) and a port to connect to a server directly wich is not limited to be on the lan network.
        bool ConnectToServer(sf::IpAddress IPOfServer, unsigned short port, size_t amoutOfConnectionAttempts = 3);

        bool send_tcp(NetMessage* message, bool encrypt = true);
        bool send_udp(NetMessage* message, bool encrypt = true);
        void Disconnect();
        void HandleChangeDN(DisplayNameUpdate* m);
        ConcurrentQueue<NetMessage*>& getQueue();
        bool IsConnected() const;
        const std::string& getDisplayName() const;
    private:
        void tcp_recive();
        void udp_recive();
        NetMessage* unwrap_msg(sf::Packet p) const;
        struct ServerDetails
        {
            sf::IpAddress IP;
            sf::Uint16 udp_port;
            sf::Uint16 tcp_port;

            ServerDetails() : IP(sf::IpAddress::None), udp_port(0), tcp_port(0){}
        };

        sf::UdpSocket m_udpSocket;
        sf::TcpSocket m_tcpSocket;
        std::string m_registredName {""};
        std::string m_displayName {""};
        ServerDetails m_serverData;
        CryptoPP::SecByteBlock m_aesKey;
        std::thread m_tcpReciveThread;
        std::thread m_udpReciveThread;
        ConcurrentQueue<NetMessage*> m_queue;
        bool m_connected{false};
};

#endif // NETWORK_H
