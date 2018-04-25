#include "Network.h"
#include <iostream>
#include <Stringhelpers.h>

Network::Network()
{
}

Network::~Network()
{
}

bool Network::ConnectToServer(sf::IpAddress IPOfServer, unsigned short port, size_t amoutOfConnectionAttempts)
{
    //Bind to any port available so the server can respond us back with a message
    if(m_udpSocket.bind(sf::Socket::AnyPort) != sf::Socket::Done)
    {
        std::cout << "Could not bind to a port" << std::endl;
        return false;
    }

    //Create the packet to send over to the server
    sf::Packet server_find_packet;
    server_find_packet << "notify-::-me-::-server";

    //Send Message to the server
    std::cout << "Broadcasting to find server" << std::endl;
    if(m_udpSocket.send(server_find_packet.getData(), server_find_packet.getDataSize()(), IPOfServer, port) != sf::Socket::Done)
    {
        std::cout << "Could not send the first packet to find the server" << std::endl;
        return false;
    }


    //Set up data to recive the msg from the server
    sf::Packet server_udp_recive;
    sf::IpAddress server_return_ip;
    unsigned short server_return_port;
    //Use a selector to make the recive not blocking forever
    sf::SocketSelector socket_selector;
    socket_selector.add(m_udpSocket);

    //Loop to attempt to recive from the socket up to X time specified when startiing the function
    for(size_t i = 0; i <= amoutOfConnectionAttempts; i++)
    {
        std::cout << "Attempt to get response from the server " << (i + 1) << std::endl;
        //Wait for 3 seconds to recive data
        if(socket_selector.wait(sf::seconds(3)))
        {
            //Get a response from the server, We would not need to check, as the selector only returns true when we can recive something, but just in case
            //we will check if its done or nor
            if(m_udpSocket.receive(server_udp_recive, server_return_ip, server_return_port) != sf::Socket::Done)
            {
                std::cout << "Recive failed. It really should not happend but it did ):" << std::endl;
                return false;
            }

            //We recived data so just break the loop
            break;
        }
    }

    //Clear the selector as we dont need to wait anymore on udp
    socket_selector.clear();

    //If we did not recive
    if(server_return_ip == sf::IpAddress::None)
    {
        std::cout << "Did not recive a response after " + toString<size_t>(amoutOfConnectionAttempts) + " attempts, server is not online!" << std::endl;
        return false;
    }

    //Set up tcp connection
    if(m_tcpSocket.connect(server_return_ip, (short unsigned int)server_find_packet.getData()) != sf::Socket::Done)
    {
        std::cout << "Could not establish a tcp socket connection" << std::endl;
        return false;
    }

    //Send data to set up the player in the server
    sf::Packet player_register_packet;
    sf::IpAddress myAdress = (IPOfServer == sf::IpAddress::Broadcast) ? sf::IpAddress::getLocalAddress() : sf::IpAddress::getPublicAddress();
    player_register_packet << "reg-::-" << myAdress.toString() << m_udpSocket.getLocalPort();


    if(m_tcpSocket.send(player_register_packet) != sf::Socket::Done)
    {
        std::cout << "Failed to send register command" << std::endl;
        return false;
    }



    return true;
}
