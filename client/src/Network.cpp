#include "Network.h"
#include <iostream>
#include <Cryptopp/rsa.h>
#include <Cryptopp/osrng.h>
#include <Cryptopp/base64.h>
#include "StringHelpers.hpp"
#include "Base64Helper.hpp"
#include "AESHelper.hpp"
#include "RSAHelper.hpp"

Network::Network()
{
}

Network::~Network()
{
    if(m_udpReciveThread.joinable())
        m_udpReciveThread.joind();
    if(m_tcpReciveThread.joinable())
        m_tcpReciveThread.joind();
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
    if(m_udpSocket.send(server_find_packet, IPOfServer, port) != sf::Socket::Done)
    {
        std::cout << "Could not send the first packet to find the server" << std::endl;
        return false;
    }


    //Clear the packet
    server_find_packet.clear();
    //Set up data to recive the msg from the server
    sf::Packet server_udp_recive;
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
            if(m_udpSocket.receive(server_udp_recive, m_serverData.IP, m_serverData.udp_port) != sf::Socket::Done)
            {
                std::cout << "Recive failed. It really should not happend but it did ):" << std::endl;
                return false;
            }
            else
            {
                //Check if we receive the correct information
                server_udp_recive >> m_serverData.tcp_port;
                if(m_serverData.tcp_port == 0)
                {
                    std::cout << "Did not recive responce from server with UDP port in it, waiting for more messages" << std::endl;
                    continue;
                }
                break;
            }
        }
    }

    //Clear the selector as we dont need to wait any more on udp
    socket_selector.clear();

    //If we did not receive
    if(m_serverData.IP == sf::IpAddress::None)

    //Set up tcp connection
    {
        std::cout << "Did not recive a response after " << amoutOfConnectionAttempts << " attempts, server is not online!" << std::endl;
        return false;
    }
    if(m_tcpSocket.connect(m_serverData.IP, m_serverData.tcp_port) != sf::Socket::Done)
    {
        std::cout << "Could not establish a tcp socket connection" << std::endl;
        return false;
    }

    //Execute ecryption HandShake
    sf::Packet handShake;
    handShake << "st-::-hsk";
    if(m_tcpSocket.send(handShake) != sf::Socket::Done)
    {
        std::cout << "Could not start encryption data interchange" << std::endl;
        return false;
    }

    sf::Packet pub_key_packet;
    if(m_tcpSocket.receive(pub_key_packet) != sf::Socket::Done)
    {
        std::cout << "Did not receive the servers pub pey" << std::endl;
        return false;
    }

    //Load the public key from the server
    std::string public_key;
    pub_key_packet >> public_key;
    Base64Helper::decode(public_key, public_key);
    CryptoPP::RSA::PublicKey server_key = RSAHelper::getPublicKeyFromPemString(public_key);

    //Generate a randome AES key and IV
    m_aesKey = AESHelper::generateKey(24);

    sf::IpAddress myAdress = (IPOfServer == sf::IpAddress::Broadcast) ? sf::IpAddress::getLocalAddress() : sf::IpAddress::getPublicAddress();
    std::string udpInfo;
    CryptoPP::ArraySource ss(m_aesKey, m_aesKey.size(), true, new CryptoPP::StringSink(udpInfo));
    udpInfo += "-::-" + StringHelpers::toString(myAdress) + "-::-" +  StringHelpers::toString(m_udpSocket.getLocalPort());
    std::string encryptedMsg = RSAHelper::encrypt(server_key, (CryptoPP::byte*)udpInfo.c_str(), udpInfo.size());

    sf::Packet encryptedAREKey;
    std::string base64Key;
    Base64Helper::encode((CryptoPP::byte*)encryptedMsg.c_str(), encryptedMsg.size(), base64Key);
    encryptedAREKey << "aesReg-::-" + base64Key;
    if(m_tcpSocket.send(encryptedAREKey) != sf::Socket::Done)
    {
        std::cout << "Could not send AES key" << std::endl;
        return false;
    }

    sf::Packet serverAESEncryptedInfo;
    if(m_tcpSocket.receive(serverAESEncryptedInfo) != sf::Socket::Done)
    {
        std::cout << "Could not recive player identifier number" << std::endl;
        return false;
    }

    std::string AESEncrypted;
    serverAESEncryptedInfo >> AESEncrypted;
    Base64Helper::decode(AESEncrypted, AESEncrypted);
    std::string serverResponse = AESHelper::decryptCombinedIV(m_aesKey, AESEncrypted);
    std::vector<std::string> returnTokens = StringHelpers::splitString(serverResponse, "-::-");
    if(returnTokens.size() != 3 && returnTokens[0] != "ok")
    {
        std::cout << "Did not recive ecxpected message when waiting for response after AES key sended" << std::endl;
        return false;
    }

    //Get the data out
    m_registredName = returnTokens[1];
    m_displayName = returnTokens[2];
    std::cout << "Displayname: " << m_displayName << ". Server Identifier Name: " << m_registredName << std::endl;

    return true;
}
