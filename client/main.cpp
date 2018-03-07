/*
  Snake ICA start project using SFML
    SFML documentation: http://www.sfml-dev.org/documentation/2.4.2/classes.php
  Windows build
    To build on Windows you need to have the correct version of CodeBlocks
      Download from here: http://www.codeblocks.org/downloads/26
      Must be the download titled "codeblocks-16.01mingw-setup.exe"
*/

#include <iostream>
#include <cstring>

#include "Game.h"
#include <SFML/Network.hpp>

int main()
{
    Game g;
    g.run();

    // Networking test: once you close the window, you should be able to test this.
    // Make sure your server is running.
    // cd DIRECTORY_WITH_THE_SERVER
    // mix run --no-halt

    // SENDING
    // TCP socket
    sf::TcpSocket tcp_socket;
    sf::Socket::Status status = tcp_socket.connect("127.0.0.1", 13000);
    if (status != sf::Socket::Done)
    {
        std::cerr << "connect failed\n";
    }

    // TCP send
    std::string buffer_out = "sending this data";

    if (tcp_socket.send(buffer_out.c_str(), buffer_out.size()) != sf::Socket::Done)
    {
        std::cerr << "sending failed" << std::endl;
    }
    std::cout << "Tcp sent" << std::endl;

    // UDP socket send
    buffer_out = "sending this too";
    sf::UdpSocket udp_socket;
    sf::IpAddress recipient = "127.0.0.1";
    unsigned short port = 13001;
    if (udp_socket.send(buffer_out.c_str(), buffer_out.size(), recipient, port) != sf::Socket::Done)
    {
        std::cerr << "Udp sending failed." << std::endl;
    }
    std::cout << "Udp sent" << std::endl;

    char buffer_in[1024];
    std::size_t received;

    // RECIEVING, hum I mean RECEIVING.
    // TCP socket receive
    if (tcp_socket.receive(buffer_in, 100, received) != sf::Socket::Done)
    {
        std::cerr << "Received failed" << std::endl;
    }
    std::cout << "Received " << received << " bytes" << std::endl;
    std::cout << buffer_in << std::endl;

    // UDP socket receive
    std::memset(buffer_in, 0, sizeof buffer_in);
    sf::IpAddress sender;
    unsigned short port_s;
    if (udp_socket.receive(buffer_in, 100, received, sender, port_s) != sf::Socket::Done)
    {
        std::cerr << "Udp receive failed." << std::endl;
    }
    std::cout << "Received " << received << " bytes from " << sender << " on port " << port << std::endl;
    std::cout << buffer_in << std::endl;


    std::cout << "Finished" << std::endl;
    return 0;
}
