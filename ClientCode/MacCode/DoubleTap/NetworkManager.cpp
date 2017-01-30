//
//  NetworkManager.cpp
//  DoubleTap
//
//  Created by BEECH, JOSHUA on 13/12/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#include "NetworkManager.hpp"

NetworkManager *NetworkManager::m_instance = NULL;

NetworkManager& NetworkManager::GetInstance()
{
    if(NULL == m_instance)
        m_instance = new NetworkManager;
    
    return *m_instance;
}

void NetworkManager::LookForServer()
{
    sf::UdpSocket BroadCastSocket;
    BroadCastSocket.bind(m_TCPPort);
    
    const char l_message[] = "Is anyone out there";
    sf::IpAddress BroadcastAddress = sf::IpAddress::Broadcast;

    
    char l_ReceivedMessage[2048];
    std::size_t l_SizeReceived;
    sf::IpAddress l_Sender;
    unsigned short l_SenderPort;
    
    sf::SocketSelector l_Selector;
    l_Selector.add(BroadCastSocket);
    
    // Figure out SF timers so I dont get stuck forever
    while(true)
    {
        if(BroadCastSocket.send(l_message, sizeof(l_message), BroadcastAddress, m_TCPPort) != sf::Socket::Done)
            return;
        

        if(l_Selector.wait(sf::seconds(1)))
        {
            BroadCastSocket.receive(l_ReceivedMessage, sizeof(l_ReceivedMessage), l_SizeReceived, l_Sender, l_SenderPort);
            break;
        }
    }

    m_ServerAddress = l_Sender;
    std::cout<<m_ServerAddress<<std::endl;
    
    sf::Socket::Status l_status = m_TCPSocket.connect(m_ServerAddress, m_TCPPort);
    
    if(l_status != sf::Socket::Done)
    {
        std::cout<<"Shit has hit the fan"<<std::endl;
    }
    
    std::thread t_ReceiveThread(&NetworkManager::BeginTCPReceive, this);
    t_ReceiveThread.detach();
}


void NetworkManager::BindUDPSocket(int p_RoomPort)
{
    m_UDPPort = p_RoomPort;
    m_UDPSocket.bind(p_RoomPort);
    m_IsBound = true;
    
    *m_ReceiveUDP = true;
    std::thread t_ReceiveUDPThread(&NetworkManager::BeginUDPReceive, this);
    t_ReceiveUDPThread.detach();
}

void NetworkManager::SendTCPMessage(std::string p_msg)
{
    char buffer[BUFFSIZE];

    memset(buffer, 0, BUFFSIZE);
    memcpy(buffer, p_msg.c_str(), p_msg.length());

    
    m_TCPSocket.send(buffer, BUFFSIZE);
}

std::string NetworkManager::ReceiveTCPMessage()
{
    char buffer[BUFFSIZE];
    std::size_t l_Received;
    memset(buffer, 0, BUFFSIZE);
    m_TCPSocket.receive(buffer, BUFFSIZE, l_Received);

    return std::string(buffer);
}

void NetworkManager::BeginTCPReceive()
{
    m_TCPMessageQueue.SetLimit(100);
    std::string l_Message;
    while(m_ReceiveTCP)
    {
        l_Message = ReceiveTCPMessage();
        if(!l_Message.empty())
            m_TCPMessageQueue.Add(l_Message);       
    
    }
}

void NetworkManager::StopReceivingTCP()
{
    *m_ReceiveTCP = false;
}

void NetworkManager::SendUDPMessage(std::string p_msg)
{
    char buffer[BUFFSIZE];
    //long n;
    memset(buffer, 0, BUFFSIZE);
    memcpy(buffer, p_msg.c_str(), p_msg.length());
    
    m_UDPSocket.send(buffer, BUFFSIZE, m_ServerAddress, m_UDPPort);
}

std::string NetworkManager::ReceiveUDPMessage()
{
    char buffer[BUFFSIZE];
    std::size_t l_Received;
    memset(buffer, 0, BUFFSIZE);

    m_UDPSocket.receive(buffer, BUFFSIZE, l_Received, m_ServerAddress, m_UDPPort);
    return std::string(buffer);
}

void NetworkManager::BeginUDPReceive()
{
    std::cout<<"Receiving UDP"<<std::endl;
    m_UDPMessageQueue.SetLimit(200);
    std::string l_Message;
    while(m_ReceiveTCP)
    {
        l_Message = ReceiveUDPMessage();
        if(!l_Message.empty())
            m_UDPMessageQueue.Add(l_Message);
        
    }
}

void NetworkManager::StopReceivingUDP()
{
    *m_ReceiveUDP = false;
}
















