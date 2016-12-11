//
//  MenuState.cpp
//  DoubleTap
//
//  Created by BEECH, JOSHUA on 10/12/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#include "MenuState.hpp"

MenuState::MenuState()
{
    Create();
}

MenuState::~MenuState()
{
    Destroy();
}

void MenuState::Create()
{
    m_ID = MENU;
    std::cout<<"Creating Menu State"<<std::endl;
}

void MenuState::Destroy()
{
    std::cout<<"Destroying Menu State"<<std::endl;
}

void MenuState::Enter()
{
    std::cout<<"Entering Menu State"<<std::endl;
}

void MenuState::Exit()
{
    std::cout<<"Leaving Menu State"<<std::endl;
}

void MenuState::Update()
{
    //std::cout<<"Updating Menu State"<<std::endl;
}

void MenuState::Render()
{
    std::cout<<"Rendering Menu State"<<std::endl;
}
