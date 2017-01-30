//
//  MenuState.cpp
//  DoubleTap
//
//  Created by BEECH, JOSHUA on 10/12/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#include "MenuState.hpp"
#include "NetworkManager.hpp"
#include "StateManager.hpp"
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
    m_ID = 0;
    m_MainGUI = p_GUI;
    m_Theme = p_Theme;
    LoadScenes();
    ShowScene(GUI_SCENE::LOGIN);
    m_CurrentScene = GUI_SCENE::LOGIN;
}

void MenuState::Destroy()
{
    std::cout<<"Destroying Menu State"<<std::endl;
}

void MenuState::Enter()
{
    ShowScene(m_CurrentScene);
    if(m_CurrentScene == GUI_SCENE::MAINLOBBY)
        RefreshRoomList();
}

void MenuState::Exit()
{
    for(auto& l_Scene : m_GUIScenes)
        HideScene(l_Scene.first);
}

void MenuState::Update()
{
    CheckMessages(1);
}

void MenuState::Render(sf::RenderWindow& p_RenderWindow)
{
    // Menu State is all TGUI based therefore no need for render
    return;
}

void MenuState::CheckMessages(int p_Protocol)
{
    // Get all messages
    // Loop through messages and act accordingly
    std:std::string l_Message;
    
    if(p_Protocol == 1)
        l_Message = NETWORK_MANAGER.GetTCPMessageQueue().Retrieve();
    else if(p_Protocol == 2)
        l_Message = NETWORK_MANAGER.GetUDPMessageQueue().Retrieve();
    
    if(l_Message.empty())
        return;
    
    std::vector<std::string> l_MessageParts = SplitString(l_Message, ':');
    
    int l_MessageType = std::stoi(l_MessageParts[0]);
    e_MenuStateMessages l_MessageHead = static_cast<e_MenuStateMessages>(l_MessageType);
    
    switch (l_MessageHead) {
        case REGISTER:
            // Fail error
            break;
        case LOGIN:
            if(l_MessageParts[1] == "Success")
            {
                HideScene(GUI_SCENE::LOGIN);
                ShowScene(GUI_SCENE::MAINLOBBY);
                m_CurrentScene = GUI_SCENE::MAINLOBBY;
                GetRoomList();
            }
            break;
        case LOGOUT:
            if(l_MessageParts[1] == "Success")
            {
                HideScene(MAINLOBBY);
                ShowScene(GUI_SCENE::LOGIN);
                m_CurrentScene = GUI_SCENE::LOGIN;
            }
        case CREATEROOM:
            // Fail error
            break;
        case JOINROOM:
            if(l_MessageParts[1] == "Success")
            {
                NETWORK_MANAGER.AssignRoom(l_MessageParts[2]);
                NETWORK_MANAGER.SendTCPMessage("Settings:MapSize:" + NETWORK_MANAGER.GetRoomName() + ":" +
                                            std::to_string((int)m_MainGUI->getSize().x) + ":" +
                                            std::to_string((int)m_MainGUI->getSize().y) +":");
                STATE_MANAGER.ChangeState(1);
            }
            break;
        case UPDATELIST:
            if(!l_MessageParts[1].empty())
            {
                tgui::ListBox::Ptr l_RoomList = m_MainGUI->get<tgui::ListBox>("MainLobby_RoomList");
                
                for(int i = 1; i < l_MessageParts.size(); i++)
                {
                    l_RoomList->addItem(l_MessageParts[i]);
                }
            }
            break;
        case REFRESHLIST:
            if(!l_MessageParts[1].empty())
            {
                tgui::ListBox::Ptr l_RoomList = m_MainGUI->get<tgui::ListBox>("MainLobby_RoomList");
                l_RoomList.get()->removeAllItems();
                
                for(int i = 1; i < l_MessageParts.size(); i++)
                {
                    l_RoomList->addItem(l_MessageParts[i]);
                }
            }
            break;
        case NETWORK:
            if(l_MessageParts[1] == "Bind" && NETWORK_MANAGER.IsUDPBound() == false)
                NETWORK_MANAGER.BindUDPSocket(std::stoi(l_MessageParts[2]));
            else if(l_MessageParts[1] == "UnBind")
                NETWORK_MANAGER.UnBindUDPSocket();
        default:
            break;
    }
}

void MenuState::ReceiveInput(sf::Event p_Event)
{
    // TGUI handles input above the input manager and so MenuState
    // Does not need functionaloty in here (currently)
}

void MenuState::LoadScenes()
{
    m_GUIScenes.insert(std::pair<GUI_SCENE, std::vector<std::shared_ptr<tgui::Widget>>>(GUI_SCENE::LOGIN, MakeLoginScene()));
    m_GUIScenes.insert(std::pair<GUI_SCENE, std::vector<std::shared_ptr<tgui::Widget>>>(GUI_SCENE::MAINLOBBY, MakeLobbyScene()));
    
    for(auto& scene: m_GUIScenes)
        HideScene(scene.first);
}


std::vector<std::shared_ptr<tgui::Widget>> MenuState::MakeLoginScene()
{
    //TODO: Add error messages
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
    LogoutButton->connect("pressed", &MenuState::Logout, this);
    m_MainGUI->add(LogoutButton, "MainLobby_Logout");
    l_SceneWidgets.push_back(LogoutButton);
    
    tgui::Button::Ptr RefreshListButton = m_Theme->load("Button");
    RefreshListButton->setSize(160, 60);
    RefreshListButton->setPosition(220, 10);
    RefreshListButton->setText("Refresh");
    RefreshListButton->connect("pressed", &MenuState::RefreshRoomList, this);
    m_MainGUI->add(RefreshListButton, "MainLobby_RefreshList");
    l_SceneWidgets.push_back(RefreshListButton);
    
    tgui::Button::Ptr CreateRoomButton = m_Theme->load("Button");
    CreateRoomButton->setSize(320, 60);
    CreateRoomButton->setPosition(430, 10);
    CreateRoomButton->setText("Create Room");
    CreateRoomButton->connect("pressed", &MenuState::MakeCreatePopUp, this);
    m_MainGUI->add(CreateRoomButton, "MainLobby_CreateRoom");
    l_SceneWidgets.push_back(CreateRoomButton);
    
    tgui::Button::Ptr JoinRoomButton = m_Theme->load("Button");
    JoinRoomButton->setSize(160, 60);
    JoinRoomButton->setPosition(800, 10);
    JoinRoomButton->setText("Join");
    JoinRoomButton->connect("pressed", &MenuState::JoinRoom, this);
    m_MainGUI->add(JoinRoomButton, "MainLobby_JoinRoom");
    l_SceneWidgets.push_back(JoinRoomButton);
    
    tgui::ListBox::Ptr RoomList = m_Theme->load("ListBox");
    RoomList->setSize(m_MainGUI->getSize().x, (m_MainGUI->getSize().y - 100));
    RoomList->setPosition(0, 100);
    RoomList->setItemHeight(30);
    m_MainGUI->add(RoomList, "MainLobby_RoomList");
    l_SceneWidgets.push_back(RoomList);
    

    
    return  l_SceneWidgets;
}

void MenuState::MakeCreatePopUp()
{
    tgui::ChildWindow::Ptr CreateRoomWindow = m_Theme->load("ChildWindow");
    CreateRoomWindow->setSize(320, 400);
    CreateRoomWindow->setPosition((m_MainGUI->getSize().x/4 + 160), m_MainGUI->getSize().y/4);
    CreateRoomWindow->setTitle("Create Room");
    m_MainGUI->add(CreateRoomWindow, "MainLobby_CreateRoomWindow");
    
    tgui::EditBox::Ptr RoomNameEditBox = m_Theme->load("EditBox");
    RoomNameEditBox->setSize(300, 50);
    RoomNameEditBox->setPosition(10, 10);
    RoomNameEditBox->setDefaultText("Room_Name");
    CreateRoomWindow->add(RoomNameEditBox, "PopUp_RoomName");
    
//    tgui::CheckBox::Ptr CheckBox = m_Theme->load("CheckBox");
//    CheckBox->setSize(25,25);
//    CheckBox->setPosition(10, 70);
//    CheckBox->setText("Private");
//    CreateRoomWindow->add(CheckBox, "PopUp_CheckBox");
//    
//    tgui::EditBox::Ptr RoomPasswordEditBox = m_Theme->load("EditBox");
//    RoomNameEditBox->setSize(300, 50);
//    RoomNameEditBox->setPosition(10, 105);
//    RoomNameEditBox->setDefaultText("Password");
//    CreateRoomWindow->add(RoomNameEditBox, "PopUp_RoomPassword");
    
    tgui::Button::Ptr CreateButton = m_Theme->load("Button");
    CreateButton->setSize(160, 50);
    CreateButton->setPosition(80, 300);
    CreateButton->setText("Create");
    CreateButton->connect("pressed", &MenuState::CreateRoom, this);
    CreateRoomWindow->add(CreateButton, "PopUp_Button");
}

void MenuState::Login()
{
    tgui::EditBox::Ptr l_UsernameBox = m_MainGUI->get<tgui::EditBox>("Menu_Username");
    tgui::EditBox::Ptr l_PasswordBox = m_MainGUI->get<tgui::EditBox>("Menu_Password");
    std::string l_Username = l_UsernameBox->getText();
    std::string l_Password = l_PasswordBox->getText();
    l_Password = sha256(l_Password);
    NETWORK_MANAGER.SendTCPMessage("login:" + l_Username + ":" + l_Password + ":");
    NETWORK_MANAGER.AssignUsername(l_Username);

}

void MenuState::Register()
{
    tgui::EditBox::Ptr l_UsernameBox = m_MainGUI->get<tgui::EditBox>("Menu_Username");
    tgui::EditBox::Ptr l_PasswordBox = m_MainGUI->get<tgui::EditBox>("Menu_Password");
    std::string l_Username = l_UsernameBox->getText();
    std::string l_Password = l_PasswordBox->getText();
    l_Password = sha256(l_Password);
    NETWORK_MANAGER.SendTCPMessage("reg:" + l_Username + ":" + l_Password + ":");
}

void MenuState::Logout()
{
    NETWORK_MANAGER.SendTCPMessage("logout:" + NETWORK_MANAGER.GetUsername() + ":");
}

void MenuState::CreateRoom()
{
    std::string l_Name = m_MainGUI->get<tgui::ChildWindow>("MainLobby_CreateRoomWindow")->get<tgui::EditBox>("PopUp_RoomName")->getText();
    NETWORK_MANAGER.SendTCPMessage("createroom:" + l_Name + ":public:" + NETWORK_MANAGER.GetUsername() +":");
    m_MainGUI->get<tgui::ChildWindow>("MainLobby_CreateRoomWindow")->destroy();
}

void MenuState::JoinRoom()
{
    tgui::ListBox::Ptr l_RoomList = m_MainGUI->get<tgui::ListBox>("MainLobby_RoomList");
    std::string l_ChosenRooom = l_RoomList->getSelectedItem();
    
    if(!l_ChosenRooom.empty())
    {
        std::vector<std::string> l_RoomDetails = SplitString(l_ChosenRooom, '\t');
        NETWORK_MANAGER.SendTCPMessage("joinroom:" + l_RoomDetails[0] + ":" + NETWORK_MANAGER.GetUsername() +":");
    }
}

void MenuState::GetRoomList()
{
    NETWORK_MANAGER.SendTCPMessage("GetRooms:");
}

void MenuState::RefreshRoomList()
{
    NETWORK_MANAGER.SendTCPMessage("RefreshRooms:");
}
