//
//  GameState.cpp
//  DoubleTap
//
//  Created by BEECH, JOSHUA on 11/12/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#include "GameState.hpp"
#include "StateManager.hpp"
#include <SFML/Window.hpp>

GameState::GameState()
{
    Create();
}

GameState::~GameState()
{
    Destroy();
}

void GameState::Create()
{
    m_ID = GAME;
    std::cout<<"Creating Game State"<<std::endl;
}

void GameState::Destroy()
{
    std::cout<<"Destroying Game State"<<std::endl;
}

void GameState::Enter()
{
    std::cout<<"Entering Game State"<<std::endl;
}

void GameState::Exit()
{
    std::cout<<"Leaving Game State"<<std::endl;
}

void GameState::Update()
{
    //std::cout<<"Updating Game State"<<std::endl;
}

void GameState::Render()
{
    std::cout<<"Rendering Game State"<<std::endl;
}
