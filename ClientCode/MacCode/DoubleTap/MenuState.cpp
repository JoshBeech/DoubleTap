//
//  MenuState.cpp
//  DoubleTap
//
//  Created by BEECH, JOSHUA on 10/12/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#include "MenuState.hpp"

MenuState::MenuState(tgui::Gui* p_GUI, std::shared_ptr<tgui::Theme> p_Theme)
{
    Create(p_GUI, p_Theme);
}

MenuState::~MenuState()
{
    Destroy();
}

void MenuState::Create(tgui::Gui* p_GUI, std::shared_ptr<tgui::Theme> p_Theme)
{
    m_ID = MENU;
    m_MainGUI = p_GUI;
    m_Theme = p_Theme;
    LoadScenes();
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

void MenuState::LoadScenes()
{
    m_GUIScenes.insert(std::pair<GUI_SCENE, std::vector<std::shared_ptr<tgui::Widget>>>(LOGIN, MakeLoginScene()));
    m_GUIScenes.insert(std::pair<GUI_SCENE, std::vector<std::shared_ptr<tgui::Widget>>>(LOBBY, MakeLobbyScene()));
}

std::vector<std::shared_ptr<tgui::Widget>> MenuState::MakeLoginScene()
{
    std::vector<std::shared_ptr<tgui::Widget>> l_SceneWidgets;
    
    tgui::EditBox::Ptr UsernameEditBox = m_Theme->load("EditBox");
    UsernameEditBox->setSize(320, 60);
    UsernameEditBox->setPosition((m_MainGUI->getSize().x - UsernameEditBox->getSize().x)/2, (m_MainGUI->getSize().y/2) - (UsernameEditBox->getSize().y *2));
    UsernameEditBox->setDefaultText("Username");
    m_MainGUI->add(UsernameEditBox, "Username");
    l_SceneWidgets.push_back(UsernameEditBox);
    
    tgui::EditBox::Ptr PasswordEditBox = m_Theme->load("EditBox");
    PasswordEditBox->setSize(320, 60);
    PasswordEditBox->setPosition((m_MainGUI->getSize().x/2) - (PasswordEditBox->getSize().x/2), (m_MainGUI->getSize().y/2) - (PasswordEditBox->getSize().y));
    PasswordEditBox->setPasswordCharacter('*');
    PasswordEditBox->setDefaultText("Password");
    m_MainGUI->add(PasswordEditBox, "Password");
    l_SceneWidgets.push_back(PasswordEditBox);
    
    tgui::Button::Ptr LoginButton = m_Theme->load("Button");
    LoginButton->setSize(160, 60);
    LoginButton->setPosition((m_MainGUI->getSize().x/2) - LoginButton->getSize().x, m_MainGUI->getSize().y/2);
    LoginButton->setText("Login");
    LoginButton->connect("pressed", [&](){std::cout<<"logging in"<<std::endl;});
    m_MainGUI->add(LoginButton, "Login");
    l_SceneWidgets.push_back(LoginButton);
    
    tgui::Button::Ptr RegisterButton = m_Theme->load("Button");
    RegisterButton->setSize(160, 60);
    RegisterButton->setPosition(m_MainGUI->getSize().x/2, m_MainGUI->getSize().y/2);
    RegisterButton->setText("Register");
    RegisterButton->connect("pressed", [&](){std::cout<<"Registering"<<std::endl;});
    m_MainGUI->add(RegisterButton, "Register");
    l_SceneWidgets.push_back(RegisterButton);
    
    return  l_SceneWidgets;
}

std::vector<std::shared_ptr<tgui::Widget>> MenuState::MakeLobbyScene()
{
    std::vector<std::shared_ptr<tgui::Widget>> l_SceneWidgets;
    
    return  l_SceneWidgets;
}

void MenuState::Login()
{
    
}
