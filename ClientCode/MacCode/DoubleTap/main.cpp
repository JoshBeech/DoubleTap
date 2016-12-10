//
//  main.cpp
//  t01
//
//  Created by Cordry, Julien on 30/09/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <thread>
#include <unistd.h>
#include "Player.hpp"

const int BUFFSIZE = 2048;

void EstablishConnection(int sockfd, std::string hostname, std::string port);
long SendMessage(std::string msg, int sockfd);
std::string ReceiveMessage(int sockfd);

//TODO:EVERY-FUCKING-THING AGAIN!!!!!!!!!!, then everything else -_-
/*
 Client:
    Add Player Controls - INPUTMANAGER
    Move Network to separate class - static?
    STATEMACHINE
    Multithread messages
    Make Map
    Add Map
    Physics#1 - collision?
    Jumping
    Weapons
    Collectables
    Menu/s
*/

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "Double Tap");
    
    //int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    //EstablishConnection(sockfd, "localhost", "27550");
    //SendMessage("hello server", sockfd);
    //std::cout<<ReceiveMessage(sockfd)<<std::endl;
    
    Player test_player(10, 10);
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        window.clear();
        window.draw(test_player.GetAvatar());
        window.display();
    }
    
    return 0;
}

void EstablishConnection(int sockfd, std::string hostname, std::string port)
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
    if ((status = getaddrinfo(hostname.c_str(),
                              port.c_str(), &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo error: %s\n",
                gai_strerror(status));
        exit(1);
    }
    // servinfo now points to a linked list of 1 or more struct addrinfos
    // ... Do everything until you don 't need servinfo anymore ...
    
    connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
    
    freeaddrinfo(servinfo); // free the linked-list
}

long SendMessage(std::string msg, int sockfd)
{
    char buffer[BUFFSIZE];
    long n;
    memset(buffer, 0, BUFFSIZE);
    memcpy(buffer, msg.c_str(), msg.length());
    n = write(sockfd, buffer, msg.length());
    // as an alternative: send
    if (n < 0)
    {
        perror("send");
        exit(-1);
    }
    return n;
}

std::string ReceiveMessage(int sockfd)
{
    char buffer[BUFFSIZE];
    // read
    memset(buffer, 0, BUFFSIZE);
    long n = read(sockfd, buffer, BUFFSIZE);
    // as an alternative: recv
    if (n < 0) 
    {
        perror("recv");	
        exit(-1);
    }
    return std::string(buffer);
}
