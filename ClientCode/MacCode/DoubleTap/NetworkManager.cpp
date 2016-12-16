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

void NetworkManager::InitaliseSocket()
{
    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
}

void NetworkManager::EstablishConnection()
{
    // Setting things up
    int status;
    struct addrinfo hints;
    struct addrinfo* servinfo;
    // will point to the results
    memset(&hints, 0, sizeof hints);
    // make sure the struct is empty
    hints.ai_family = AF_INET;
    // Choose IPv4 over IPv6
    hints.ai_socktype = SOCK_STREAM;
    // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;
    // fill in my IP for me OR use NULL
    if ((status = getaddrinfo(m_hostname.c_str(),
                              m_port.c_str(), &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo error: %s\n",
                gai_strerror(status));
        _exit(1);
    }
    // servinfo now points to a linked list of 1 or more struct addrinfos
    // ... Do everything until you don 't need servinfo anymore ...
    
    connect(m_sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
    
    freeaddrinfo(servinfo); // free the linked-list
}

long NetworkManager::SendMessage(std::string p_msg)
{
    char buffer[BUFFSIZE];
    long n;
    memset(buffer, 0, BUFFSIZE);
    memcpy(buffer, p_msg.c_str(), p_msg.length());
    n = write(m_sockfd, buffer, p_msg.length());
    // as an alternative: send
    if (n < 0)
    {
        perror("send");
        _exit(-1);
    }
    return n;
}

std::string NetworkManager::ReceiveMessage()
{
    char buffer[BUFFSIZE];
    // read
    memset(buffer, 0, BUFFSIZE);
    long n = read(m_sockfd, buffer, BUFFSIZE);
    // as an alternative: recv
    if (n < 0)
    {
        perror("recv");
        _exit(-1);
    }
    return std::string(buffer);
}
