#include "Network.h"
#include <iostream>
#include <Cryptopp/rsa.h>
#include <Cryptopp/osrng.h>
#include "StringHelpers.hpp"
#include "Base64Helper.hpp"
#include "AESHelper.hpp"
#include "RSAHelper.hpp"
#include "Logger.hpp"
#include "EmptyNetMessage.hpp"
#include "DisplayNameUpdate.hpp"
#include "GameLobbyUpdateNetMessage.hpp"
#include "StartGameNetMessage.hpp"
#include "PlayerStatusUpdateNetMessage.hpp"
#include "BulletInstanciateNetMessage.hpp"
#include "HPPlayerUpdateNetMessage.hpp"
#include "PlayerBulletCountUpdateNetMessage.hpp"
#include "PlayerDieNetMessage.hpp"
#include "PlayerRespawnNetMessage.hpp"

Network::Network()
{
}

Network::~Network()
{
    Disconnect();
}

bool Network::ConnectToServer(sf::IpAddress IPOfServer, unsigned short port, size_t amoutOfConnectionAttempts)
{
    //Bind to any port available so the server can respond us back with a message
    if(m_udpSocket.bind(sf::Socket::AnyPort) != sf::Socket::Done)
    {
        Disconnect();
        Log(l_CRITICAL) << "Could not bind to a port";
        return false;
    }

    //Create the packet to send over to the server
    sf::Packet server_find_packet;
    server_find_packet << "notify-::-me-::-server";

    //Send Message to the server
    Log(l_INFO) << "Broadcasting to find server";
    if(m_udpSocket.send(server_find_packet, IPOfServer, port) != sf::Socket::Done)
    {
        Disconnect();
        Log(l_CRITICAL) << "Could not send the first packet to find the server";
        return false;
    }


    //Clear the packet
    server_find_packet.clear();
    //Set up data to recive the msg from the server
    sf::Packet server_udp_recive;
    //Use a selector to make the recive not blocking forever
    sf::SocketSelector socket_selector;
    socket_selector.add(m_udpSocket);

    std::string myIpFromServer;
    //Loop to attempt to recive from the socket up to X time specified when startiing the function
    for(size_t i = 0; i < amoutOfConnectionAttempts; i++)
    {
        Log(l_DEBUG) << "Attempt to get response from the server " << (i + 1);
        //Wait for 3 seconds to recive data
        if(socket_selector.wait(sf::seconds(3)))
        {
            //Get a response from the server, We would not need to check, as the selector only returns true when we can recive something, but just in case
            //we will check if its done or nor
            if(m_udpSocket.receive(server_udp_recive, m_serverData.IP, m_serverData.udp_port) != sf::Socket::Done)
            {
                Disconnect();
                Log(l_CRITICAL) << "Recive failed. It really should not happend but it did ):";
                return false;
            }
            else
            {
                //Check if we receive the correct information
                server_udp_recive >> m_serverData.tcp_port >> myIpFromServer;
                if(m_serverData.tcp_port == 0)
                {
                    Disconnect();
                    Log(l_CRITICAL) << "Did not recive responce from server with UDP port in it, waiting for more messages";
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
    {
        Disconnect();
        Log(l_WARN) << "Did not recive a response after " << amoutOfConnectionAttempts << " attempts, server is not online!";
        return false;
    }
    if(m_tcpSocket.connect(m_serverData.IP, m_serverData.tcp_port) != sf::Socket::Done)
    {
        Disconnect();
        Log(l_CRITICAL) << "Could not establish a tcp socket connection";
        return false;
    }

    //Execute ecryption HandShake
    sf::Packet handShake;
    handShake << "st-::-hsk";
    if(m_tcpSocket.send(handShake) != sf::Socket::Done)
    {
        Disconnect();
        Log(l_CRITICAL) << "Could not start encryption data interchange";
        return false;
    }

    sf::Packet pub_key_packet;
    if(m_tcpSocket.receive(pub_key_packet) != sf::Socket::Done)
    {
        Disconnect();
        Log(l_CRITICAL) << "Did not receive the servers pub pey";
        return false;
    }

    //Load the public key from the server
    std::string public_key;
    pub_key_packet >> public_key;
    Base64Helper::decode(public_key, public_key);
    CryptoPP::RSA::PublicKey server_key = RSAHelper::getPublicKeyFromPemString(public_key);

    //Generate a randome AES key and IV
    m_aesKey = AESHelper::generateKey(24);


    std::string udpInfo;
    CryptoPP::ArraySource ss(m_aesKey, m_aesKey.size(), true, new CryptoPP::StringSink(udpInfo));
    udpInfo += "-::-" + myIpFromServer + "-::-" +  StringHelpers::toString(m_udpSocket.getLocalPort());
    std::string encryptedMsg = RSAHelper::encrypt(server_key, (CryptoPP::byte*)udpInfo.c_str(), udpInfo.size());

    sf::Packet encryptedAREKey;
    std::string base64Key;
    Base64Helper::encode((CryptoPP::byte*)encryptedMsg.c_str(), encryptedMsg.size(), base64Key);
    encryptedAREKey << "aesReg-::-" + base64Key;
    if(m_tcpSocket.send(encryptedAREKey) != sf::Socket::Done)
    {
        Disconnect();
        Log(l_CRITICAL) << "Could not send AES key";
        return false;
    }

    sf::Packet serverAESEncryptedInfo;
    if(m_tcpSocket.receive(serverAESEncryptedInfo) != sf::Socket::Done)
    {
        Disconnect();
        Log(l_CRITICAL) << "Could not recive player identifier number";
        return false;
    }

    std::string AESEncrypted;
    serverAESEncryptedInfo >> AESEncrypted;
    Base64Helper::decode(AESEncrypted, AESEncrypted);
    std::string serverResponse = AESHelper::decryptCombinedIV(m_aesKey, AESEncrypted);
    std::vector<std::string> returnTokens = StringHelpers::splitString(serverResponse, "-::-");
    if(returnTokens.size() != 3 && returnTokens[0] != "ok")
    {
        Disconnect();
        Log(l_CRITICAL) << "Did not recive ecxpected message when waiting for response after AES key sended";
        Log(l_CRITICAL) << "Recived" << serverResponse;
        return false;
    }

    //Get the data out
    m_registredName = returnTokens[1];
    m_displayName = returnTokens[2];
    Log(l_INFO) << "Displayname: " << m_displayName << ". Server Identifier Name: " << m_registredName;

    m_connected = true;

    m_tcpReciveThread = std::thread(&Network::tcp_recive, this);
    m_udpReciveThread = std::thread(&Network::udp_recive, this);

    return true;
}

bool Network::send_tcp(NetMessage* message, bool encrypt)
{
    if(message == nullptr)
        return false;

    bool br = true;

    std::string str_toSend = m_registredName + "-::-";
    if(encrypt)
    {
        str_toSend += "1-::-" + StringHelpers::convertPacketToString(message->BuildEncryptPacket(m_aesKey));
    }
    else
    {
        str_toSend += StringHelpers::convertPacketToString(message->BuildPacket());
    }
    Log(l_DEBUG) <<  "Sending over tcp: " << str_toSend;
    sf::Packet pToSend;
    pToSend << str_toSend;
    if(m_tcpSocket.send(pToSend) != sf::Socket::Done)
    {
        Log(l_CRITICAL) << "Could not send tcp packet to the server";
        br = false;
    }
    delete message;
    return br;
}

bool Network::send_udp(NetMessage* message, bool encrypt)
{
    if(message == nullptr)
        return false;

    bool br = true;
    std::string str_toSend = m_registredName + "-::-";
    if(encrypt)
    {
        str_toSend += "1-::-" + StringHelpers::convertPacketToString(message->BuildEncryptPacket(m_aesKey));
    }
    else
    {
        str_toSend += StringHelpers::convertPacketToString(message->BuildPacket());
    }
    Log(l_DEBUG) <<  "Sending over udp: " << str_toSend;
    sf::Packet pToSend;
    pToSend << str_toSend;
    if(m_udpSocket.send(pToSend, m_serverData.IP, m_serverData.udp_port) != sf::Socket::Done)
    {
        Log(l_CRITICAL) << "Could not send udp packet to the server";
        br = false;
    }
    delete message;
    return br;
}

void Network::HandleChangeDN(DisplayNameUpdate* m)
{
    if(m == nullptr)
        return;

    m_displayName = m->DisplayName;
    delete m;
}


void Network::Disconnect()
{
    m_connected = false;
    m_tcpSocket.disconnect();
    m_udpSocket.unbind();
    if(m_udpReciveThread.joinable())
        m_udpReciveThread.join();
    if(m_tcpReciveThread.joinable())
        m_tcpReciveThread.join();
}

ConcurrentQueue<NetMessage*>& Network::getQueue()
{
    return m_queue;
}

bool Network::IsConnected() const
{
    return m_connected;
}

const std::string& Network::getDisplayName() const
{
    return m_displayName;
}

void Network::tcp_recive()
{
    sf::Packet p;
    while(m_connected)
    {
        p.clear();
        sf::Socket::Status s = m_tcpSocket.receive(p);
        if(s == sf::Socket::Done)
        {
            m_queue.push(unwrap_msg(p));
        }
    }
}

void Network::udp_recive()
{
    sf::Packet p;
    Network::ServerDetails incomingDT;
    while(m_connected)
    {
        p.clear();
        if(m_udpSocket.receive(p, incomingDT.IP, incomingDT.udp_port) == sf::Socket::Done && incomingDT.IP == m_serverData.IP && incomingDT.udp_port == m_serverData.udp_port)
        {
            Log() << "Recived msg udp";
            m_queue.push(unwrap_msg(p));
        }
    }
}
NetMessage* Network::unwrap_msg(sf::Packet p) const
{
    NetMessage* returnM = nullptr;
    sf::Uint16 type;
    p >> type;
    Log(l_DEBUG) << "Got message of type: " << type;
    switch((eNetMessageType)type)
    {
    case eNetMessageType::eEncrypted:
        {
            std::string chiper;
            p >> chiper;
            Base64Helper::decode(chiper, chiper);
            sf::Packet decryptedP;
            std::string plain = AESHelper::decryptCombinedIV(m_aesKey, chiper);
            decryptedP.clear();
            decryptedP.append(plain.data(), plain.size());
            returnM = unwrap_msg(decryptedP);
            return returnM;
            break;
        }
    case eNetMessageType::eDisplayNameUpdate:
        {
            returnM = new DisplayNameUpdate();
            break;
        }
    case eNetMessageType::eGameLobbyUpdated:
        {
            returnM = new GameLobbyUpdateNetMessage();
            break;
        }
    case eNetMessageType::eGameLobbyStartGame:
        {
            returnM = new StartGameNetMessage();
            break;
        }
    case eNetMessageType::eInGamePlayerPosUpdate:
        {
            returnM = new PlayerStatusUpdateNetMessage();
            break;
        }
    case eNetMessageType::eBulletInstanciateMessage:
        {
            returnM = new BulletInstanciateNetMessage();
            break;
        }
    case eNetMessageType::eHpPlayerUpdate:
        {
            returnM = new HPPlayerUpdateNetMessage();
            break;
        }
    case eNetMessageType::ePlayerBulletCountUpdate:
        {
            returnM = new PlayerBulletCountUpdateNetMessage();
            break;
        }
    case eNetMessageType::ePlayerRespanwsMessage:
        {
            returnM = new PlayerRespawnNetMessage();
            break;
        }
    case eNetMessageType::ePlayerDiesMessage:
        {
            returnM = new PlayerDieNetMessage();
            break;
        }
    }
    if((returnM == nullptr))
    {
        returnM = new EmptyNetMessage();
    }
    returnM->BuildMessage(p);
    return  returnM;
}

