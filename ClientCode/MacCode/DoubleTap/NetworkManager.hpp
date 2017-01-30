//
//  NetworkManager.hpp
//  DoubleTap
//
//  Created by BEECH, JOSHUA on 13/12/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#pragma once

#include <SFML/Network.hpp>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <thread>
#include <unistd.h>
#include <string>
#include <iostream>
#include "MessageQueue.hpp"

class NetworkManager
{
public:
    static NetworkManager& GetInstance();
    ~NetworkManager() {};
    void LookForServer();
    void BindUDPSocket(int p_RoomPort);
    void UnBindUDPSocket() {m_UDPSocket.unbind();}
    bool IsUDPBound() {return m_IsBound;}
    
    void SendTCPMessage(std::string p_msg);
    std::string ReceiveTCPMessage();
    void BeginTCPReceive();
    void StopReceivingTCP();
    void SendUDPMessage(std::string p_msg);
    std::string ReceiveUDPMessage();
    void BeginUDPReceive();
    void StopReceivingUDP();
    MessageQueue<std::string>& GetTCPMessageQueue() {return m_TCPMessageQueue;}
    MessageQueue<std::string>& GetUDPMessageQueue() {return m_UDPMessageQueue;}
    
    const int& GetTCPPort() { return m_TCPPort; }
    
    const std::string& GetUsername() {return m_Username;}
    void AssignUsername(std::string p_Username) {m_Username = p_Username;}
    const std::string& GetRoomName() {return m_RoomName;}
    void AssignRoom(std::string p_RoomName) {m_RoomName = p_RoomName;}
    const std::string& GetReadyStatus() {return m_ReadyStatus;}
    void SetStatus(std::string p_NewStatus) {m_ReadyStatus = p_NewStatus;}
    const std::string& GetTeam() {return m_Team;}
    void SetTeam(std::string p_NewTeam) {m_Team = p_NewTeam;}
protected:
    NetworkManager() {};
    
private:
    static NetworkManager* m_instance;
    sf::IpAddress m_ServerAddress;
    sf::TcpSocket m_TCPSocket;
    sf::UdpSocket m_UDPSocket;
    int m_TCPPort = 27550;
    unsigned short m_UDPPort;
    bool m_IsBound = false;
    std::size_t BUFFSIZE = 2048;
    std::string m_Username;
    std::string m_RoomName;
    std::string m_ReadyStatus;
    std::string m_Team;
    
    MessageQueue<std::string> m_TCPMessageQueue;
    MessageQueue<std::string> m_UDPMessageQueue;
    
    std::unique_ptr<bool> m_ReceiveTCP = std::unique_ptr<bool>(new bool(true));
    std::unique_ptr<bool> m_ReceiveUDP = std::unique_ptr<bool>(new bool(true));
};

#define NETWORK_MANAGER NetworkManager::GetInstance()
