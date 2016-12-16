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

GameState::GameState(tgui::Gui* p_GUI, std::shared_ptr<tgui::Theme> p_Theme)
{
    Create(p_GUI, p_Theme);
}

GameState::~GameState()
{
    Destroy();
}

void GameState::Create(tgui::Gui* p_GUI, std::shared_ptr<tgui::Theme> p_Theme)
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

void GameState::LoadScenes()
{
    
}
