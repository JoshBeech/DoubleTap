//
//  NetworkManager.hpp
//  DoubleTap
//
//  Created by BEECH, JOSHUA on 13/12/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <thread>
#include <unistd.h>
#include <string>

class NetworkManager
{
public:
    static NetworkManager& GetInstance();
    ~NetworkManager() {};
    void InitaliseSocket();
    void EstablishConnection();
    void SendMessage(std::string p_msg);
    std::string ReceiveMessage();
    
    int GetSocket() { return m_sockfd; }
    std::string GetHostname() { return  m_hostname; }
    std::string GetPort() { return m_port; }
protected:
    NetworkManager() {};
    
private:
    static NetworkManager* m_instance;
    std::string m_hostname = "localhost";
    std::string m_port = "27550";
    int m_sockfd;
    const int BUFFSIZE = 2048;
};

#define NETWORK_MANAGER NetworkManager::GetInstance()
