//
//  main.cpp
//  t01
//
//  Created by Cordry, Julien on 30/09/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#include <iostream>
#include <SFML/Graphics.hpp>
//#include <sys/socket.h>
//#include <sys/types.h>
//#include <netdb.h>
//#include <thread>
//#include <unistd.h>
#include "NetworkManager.hpp"
#include "StateManager.hpp"
#include "MenuState.hpp"
#include "GameState.hpp"
#include "Player.hpp"


//const int BUFFSIZE = 2048;

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
    sf::RenderWindow window(sf::VideoMode(1280, 800), "Double Tap");

    std::shared_ptr<tgui::Theme> l_Theme = std::make_shared<tgui::Theme>("Themes/Black.txt");
    tgui::Gui GUI(window);
    
    std::shared_ptr<MenuState> Menu(new MenuState(&GUI, l_Theme));
    std::shared_ptr<GameState> Game(new GameState(&GUI, l_Theme));
    
    STATE_MANAGER.AddState(Menu);
    STATE_MANAGER.AddState(Game);
    //int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    //EstablishConnection(sockfd, "localhost", "27550");
    //SendMessage("hello server", sockfd);
    //std::cout<<ReceiveMessage(sockfd)<<std::endl;
    
    NETWORK_MANAGER.InitaliseSocket();
    NETWORK_MANAGER.EstablishConnection();
    //NETWORK_MANAGER.SendMessage("sendto:bob:Hi");
    
    Player test_player(10, 10);
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
            GUI.handleEvent(event);
            
            if(event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Key::Space)
                {
                    if(STATE_MANAGER.GetCurrentState()->GetID() == MENU)
                        STATE_MANAGER.ChangeState(GAME);
                    else if (STATE_MANAGER.GetCurrentState()->GetID() == GAME)
                        STATE_MANAGER.ChangeState(MENU);
                }
            }
        }
        
        STATE_MANAGER.GetCurrentState()->Update();
    
        window.clear();
        GUI.draw();
        //window.draw(test_player.GetAvatar());
        window.display();
    }
    
    delete &STATE_MANAGER;
    delete &NETWORK_MANAGER;
    
    return 0;
}
