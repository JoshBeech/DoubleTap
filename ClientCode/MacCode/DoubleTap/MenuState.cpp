//
//  MenuState.cpp
//  DoubleTap
//
//  Created by BEECH, JOSHUA on 10/12/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#include "MenuState.hpp"
#include "NetworkManager.hpp"
#include "SHA256.hpp"

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
    ShowScene(LOGIN);
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
    m_GUIScenes.insert(std::pair<GUI_SCENE, std::vector<std::shared_ptr<tgui::Widget>>>(MAINLOBBY, MakeLobbyScene()));
    
    for(auto& scene: m_GUIScenes)
        HideScene(scene.first);
}

std::vector<std::shared_ptr<tgui::Widget>> MenuState::MakeLoginScene()
{
    std::vector<std::shared_ptr<tgui::Widget>> l_SceneWidgets;
    
    tgui::EditBox::Ptr UsernameEditBox = m_Theme->load("EditBox");
    UsernameEditBox->setSize(320, 60);
    UsernameEditBox->setPosition((m_MainGUI->getSize().x - UsernameEditBox->getSize().x)/2, (m_MainGUI->getSize().y/2) - (UsernameEditBox->getSize().y *2));
    UsernameEditBox->setDefaultText("Username");
    m_MainGUI->add(UsernameEditBox, "Menu_Username");
    l_SceneWidgets.push_back(UsernameEditBox);
    
    tgui::EditBox::Ptr PasswordEditBox = m_Theme->load("EditBox");
    PasswordEditBox->setSize(320, 60);
    PasswordEditBox->setPosition((m_MainGUI->getSize().x/2) - (PasswordEditBox->getSize().x/2), (m_MainGUI->getSize().y/2) - (PasswordEditBox->getSize().y));
    PasswordEditBox->setPasswordCharacter('*');
    PasswordEditBox->setDefaultText("Password");
    m_MainGUI->add(PasswordEditBox, "Menu_Password");
    l_SceneWidgets.push_back(PasswordEditBox);
    
    tgui::Button::Ptr LoginButton = m_Theme->load("Button");
    LoginButton->setSize(160, 60);
    LoginButton->setPosition((m_MainGUI->getSize().x/2) - LoginButton->getSize().x, m_MainGUI->getSize().y/2);
    LoginButton->setText("Login");
    LoginButton->connect("pressed", &MenuState::Login, this);
    m_MainGUI->add(LoginButton, "Menu_Login");
    l_SceneWidgets.push_back(LoginButton);
    
    tgui::Button::Ptr RegisterButton = m_Theme->load("Button");
    RegisterButton->setSize(160, 60);
    RegisterButton->setPosition(m_MainGUI->getSize().x/2, m_MainGUI->getSize().y/2);
    RegisterButton->setText("Register");
    RegisterButton->connect("pressed", &MenuState::Register, this);
    m_MainGUI->add(RegisterButton, "Menu_Register");
    l_SceneWidgets.push_back(RegisterButton);
    
    return  l_SceneWidgets;
}

std::vector<std::shared_ptr<tgui::Widget>> MenuState::MakeLobbyScene()
{
    std::vector<std::shared_ptr<tgui::Widget>> l_SceneWidgets;
    
    // GUI Objetcs
    // Buttons:- Create Room, Join Room, Back, Refresh
    // Room List
    
    tgui::Button::Ptr LogoutButton = m_Theme->load("Button");
    LogoutButton->setSize(160, 60);
    LogoutButton->setPosition(10, 10);
    LogoutButton->setText("Logout");
    LogoutButton->connect("pressed", [&](){std::cout<<"Loging Out"<<std::endl;});
    m_MainGUI->add(LogoutButton, "MainLobby_Logout");
    l_SceneWidgets.push_back(LogoutButton);
    
    tgui::Button::Ptr RefreshListButton = m_Theme->load("Button");
    RefreshListButton->setSize(160, 60);
    RefreshListButton->setPosition(220, 10);
    RefreshListButton->setText("Refresh");
    RefreshListButton->connect("pressed", [&](){std::cout<<"Refreshing List"<<std::endl;});
    m_MainGUI->add(RefreshListButton, "MainLobby_RefreshList");
    l_SceneWidgets.push_back(RefreshListButton);
    
    tgui::Button::Ptr CreateRoomButton = m_Theme->load("Button");
    CreateRoomButton->setSize(320, 60);
    CreateRoomButton->setPosition(430, 10);
    CreateRoomButton->setText("Create Room");
    CreateRoomButton->connect("pressed", [&](){std::cout<<"Creating New Room"<<std::endl;});
    m_MainGUI->add(CreateRoomButton, "MainLobby_CreateRoom");
    l_SceneWidgets.push_back(CreateRoomButton);
    
    tgui::Button::Ptr JoinRoomButton = m_Theme->load("Button");
    JoinRoomButton->setSize(160, 60);
    JoinRoomButton->setPosition(800, 10);
    JoinRoomButton->setText("Join");
    JoinRoomButton->connect("pressed", [&](){std::cout<<"Creating New Room"<<std::endl;});
    m_MainGUI->add(JoinRoomButton, "MainLobby_JoinRoom");
    l_SceneWidgets.push_back(JoinRoomButton);
    
    tgui::ListBox::Ptr RoomList = m_Theme->load("ListBox");
    RoomList->setSize(m_MainGUI->getSize().x, (m_MainGUI->getSize().y - 100));
    RoomList->setPosition(0, 100);
    RoomList->setItemHeight(30);
    RoomList->addItem("Room1\t 1/4 \tpublic");
    RoomList->addItem("Room2\t 2/4 \tprivate");
    RoomList->addItem("Room3\t 1/4 \tpublic");
    m_MainGUI->add(RoomList, "MainLobby_RoomList");
    l_SceneWidgets.push_back(RoomList);
    
    return  l_SceneWidgets;
}

void MenuState::Login()
{
    tgui::EditBox::Ptr l_UsernameBox = m_MainGUI->get<tgui::EditBox>("Menu_Username");
    tgui::EditBox::Ptr l_PasswordBox = m_MainGUI->get<tgui::EditBox>("Menu_Password");
    std::string l_Username = l_UsernameBox->getText();
    std::string l_Password = l_PasswordBox->getText();
    l_Password = sha256(l_Password);
    NETWORK_MANAGER.SendMessage("login:" + l_Username + ":" + l_Password + ":");
    HideScene(LOGIN);
    ShowScene(MAINLOBBY);
}

void MenuState::Register()
{
    tgui::EditBox::Ptr l_UsernameBox = m_MainGUI->get<tgui::EditBox>("Menu_Username");
    tgui::EditBox::Ptr l_PasswordBox = m_MainGUI->get<tgui::EditBox>("Menu_Password");
    std::string l_Username = l_UsernameBox->getText();
    std::string l_Password = l_PasswordBox->getText();
    l_Password = sha256(l_Password);
    NETWORK_MANAGER.SendMessage("reg:" + l_Username + ":" + l_Password + ":");
    HideScene(LOGIN);
    ShowScene(MAINLOBBY);
}


