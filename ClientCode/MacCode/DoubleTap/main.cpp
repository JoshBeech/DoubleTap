//
//  main.cpp
//  t01
//
//  Created by Cordry, Julien on 30/09/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#include <iostream>
#include <thread>
#include "NetworkManager.hpp"
#include "StateManager.hpp"
#include "MenuState.hpp"
#include "GameState.hpp"
#include "InputManager.hpp"
#include "Player.hpp"


//TODO:PANIC!!!!!!!!!!
//TODO:Stop panicing
/*
 Client:
    Add Player Controls - INPUTMANAGER
    Multithread messages
    Weapons
    Collectables
    Menu/s
    Make Map
    Add Map
    Physics#1 - collision?
    Jumping
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
    
    NETWORK_MANAGER.LookForServer();
    //NETWORK_MANAGER.InitaliseSocket();
    //NETWORK_MANAGER.EstablishConnection();
    
    InputManager l_InputManager;
    
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
                if(l_InputManager.IsEventTracked(event))
                    STATE_MANAGER.GetCurrentState()->ReceiveInput(event);
            }
        }
        
        window.clear();
        STATE_MANAGER.GetCurrentState()->Update();
        STATE_MANAGER.GetCurrentState()->Render(window);
        GUI.draw();
        window.display();
    }
    
    NETWORK_MANAGER.StopReceivingTCP();
    delete &STATE_MANAGER;
    delete &NETWORK_MANAGER;
    
    return 0;
}

