//
//  GameState.cpp
//  DoubleTap
//
//  Created by BEECH, JOSHUA on 11/12/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#include "GameState.hpp"
#include "NetworkManager.hpp"
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
    m_ID = 1;
    m_MainGUI = p_GUI;
    m_Theme = p_Theme;
    LoadScenes();
    m_CurrentScene = GUI_SCENE::ROOMLOBBY;
}

void GameState::Destroy()
{
    std::cout<<"Destroying Game State"<<std::endl;
}

void GameState::Enter()
{
    std::cout<<"Entering Game State"<<std::endl;
    ShowScene(m_CurrentScene);
    NETWORK_MANAGER.SendTCPMessage("GetPlayers:" + NETWORK_MANAGER.GetRoomName() + ":");
}

void GameState::Exit()
{
    for(auto& l_Scene : m_GUIScenes)
        HideScene(l_Scene.first);
}

void GameState::Update()
{
    CheckMessages(1);
    CheckMessages(2);
    
    if(m_InMenu == false)
    {
        NETWORK_MANAGER.SendUDPMessage("Positions:" + NETWORK_MANAGER.GetUsername() + ":" + std::to_string((int)m_User.GetPosition().x) + ":" + std::to_string((int)m_User.GetPosition().y) +":");
        
        m_User.Move();
        
        for(auto& l_Bullet : m_Bullets)
            l_Bullet.Move();
    }
}

void GameState::Render(sf::RenderWindow& p_RenderWindow)
{
    if(m_InMenu == false)
    {
        p_RenderWindow.draw(m_User.GetAvatar());
        for(auto& l_Bullet : m_Bullets)
            p_RenderWindow.draw(l_Bullet.GetAvatar());
    }
    else
    {
        return;
    }

}

void GameState::CheckMessages(int p_Protocol)
{
    std:std::string l_Message;
    
    if(p_Protocol == 1)
        l_Message = NETWORK_MANAGER.GetTCPMessageQueue().Retrieve();
    else if(p_Protocol == 2)
        l_Message = NETWORK_MANAGER.GetTCPMessageQueue().Retrieve();
        
    if(l_Message.empty())
        return;
    
    std::vector<std::string> l_MessageParts = SplitString(l_Message, ':');
    
    int l_MessageType = std::stoi(l_MessageParts[0]);
    e_GameStateMessages l_MessageHead = static_cast<e_GameStateMessages>(l_MessageType);

    switch (l_MessageHead)
    {
        case NETWORK:
            if(l_MessageParts[1] == "Bind" && NETWORK_MANAGER.IsUDPBound() == false)
                NETWORK_MANAGER.BindUDPSocket(std::stoi(l_MessageParts[2]));
            else if(l_MessageParts[1] == "UnBind")
                NETWORK_MANAGER.UnBindUDPSocket();
            break;
        case ROOMUPDATE:
            if(l_MessageParts[1] == "Lobby")
            {
                tgui::ListBox::Ptr l_Players = m_MainGUI->get<tgui::ListBox>("GameLobby_PlayerList");
                l_Players->removeAllItems();
                
                for(int i = 2; i < l_MessageParts.size(); i++)
                {
                    l_Players->addItem(l_MessageParts[i]);
                    std::vector<std::string> l_PlayerInfo = SplitString(l_MessageParts[i], '\t');
                    if(l_PlayerInfo[0] == NETWORK_MANAGER.GetUsername())
                    {
                        NETWORK_MANAGER.SetTeam(l_PlayerInfo[1]);
                        NETWORK_MANAGER.SetStatus(l_PlayerInfo[2]);
                    }
                }
            }
            break;
        case LEAVEROOM:
            if(l_MessageParts[1] == "Success")
            {
                STATE_MANAGER.ChangeState(0);
            }
            break;
        case CHAT:
            break;
        case GAME:
            if(l_MessageParts[1] == "Start")
            {
                StartGame();
            }
            else if(l_MessageParts[1] == "Fire")
            {
                Bullet l_TempBullet((m_User.GetPosition() + m_User.GetVelocity() * 5.0f), m_User.GetVelocity() * 2.0f);
                m_Bullets.push_back(l_TempBullet);
            }
            break;
        case POSITION:
            if(l_MessageParts[1] == "Self")
            {
                std::vector<std::string> l_Positions = SplitString(l_MessageParts[2], ',');
                m_User.SetPosition(sf::Vector2f(std::stoi(l_Positions[0]), std::stoi(l_Positions[1])));
            }
            break;
        case COLLECTABLE:
            break;
        case COLLISION:
            break;
        default:
            break;
    }
}

void GameState::ReceiveInput(sf::Event p_Event)
{
    if(p_Event.key.code == sf::Keyboard::Key::Left)
    {
        m_User.SetVelocity(sf::Vector2f(-0.1, 0));
    }
    else if(p_Event.key.code == sf::Keyboard::Key::Right)
    {
        m_User.SetVelocity(sf::Vector2f(0.1, 0));
    }
    else if(p_Event.key.code == sf::Keyboard::Key::Up)
    {
        m_User.SetVelocity(sf::Vector2f(0, -0.1));
    }
    else if(p_Event.key.code == sf::Keyboard::Key::Down)
    {
        m_User.SetVelocity(sf::Vector2f(0, 0.1));
    }
    
    if (p_Event.key.code == sf::Keyboard::Key::Space)
    {
        NETWORK_MANAGER.SendTCPMessage("FIRE:");
    }
}

void GameState::LoadScenes()
{
    m_GUIScenes.insert(std::pair<GUI_SCENE, std::vector<std::shared_ptr<tgui::Widget>>>(GUI_SCENE::ROOMLOBBY, MakeLobbyScene()));
    m_GUIScenes.insert(std::pair<GUI_SCENE, std::vector<std::shared_ptr<tgui::Widget>>>(GUI_SCENE::MAINGAME, MakeMainGameScene()));
    m_GUIScenes.insert(std::pair<GUI_SCENE, std::vector<std::shared_ptr<tgui::Widget>>>(GUI_SCENE::ENDGAME, MakeEndGameScene()));
    
    for(auto& scene: m_GUIScenes)
        HideScene(scene.first);
}

std::vector<std::shared_ptr<tgui::Widget>> GameState::MakeLobbyScene()
{
    std::vector<std::shared_ptr<tgui::Widget>> l_SceneWidgets;
    
    // GUI Objetcs
    // Buttons:- Exit, Ready, Player List, Chat Box Game Mode (Combo box for host?), Map image?
    // Room List
    
    tgui::Button::Ptr ExitButton = m_Theme->load("Button");
    ExitButton->setSize(160, 50);
    ExitButton->setPosition(10, 10);
    ExitButton->setText("Leave");
    ExitButton->connect("pressed", &GameState::ExitToMainLobby, this);
    m_MainGUI->add(ExitButton, "GameLobby_ExitButton");
    l_SceneWidgets.push_back(ExitButton);
    
    tgui::Button::Ptr ReadyButton = m_Theme->load("Button");
    ReadyButton->setSize(160, 50);
    ReadyButton->setPosition((m_MainGUI->getSize().x - 170), (m_MainGUI->getSize().y - 60));
    ReadyButton->setText("Ready");
    ReadyButton->connect("pressed", &GameState::ReadyUp, this);
    m_MainGUI->add(ReadyButton, "GameLobby_ReadyButton");
    l_SceneWidgets.push_back(ReadyButton);
    
    tgui::Label::Ptr GameModeLabel = m_Theme->load("Label");
    GameModeLabel->setSize(320, 60);
    GameModeLabel->setPosition(m_MainGUI->getSize().x/4 * 3, 100);
    GameModeLabel->setText("Game Mode");
    m_MainGUI->add(GameModeLabel, "GameLobby_GameModeLabel");
    l_SceneWidgets.push_back(GameModeLabel);
    
    tgui::ListBox::Ptr PlayerList = m_Theme->load("ListBox");
    PlayerList->setSize(m_MainGUI->getSize().x/3, 240);
    PlayerList->setPosition(0, 100);
    PlayerList->setItemHeight(30);
    m_MainGUI->add(PlayerList, "GameLobby_PlayerList");
    l_SceneWidgets.push_back(PlayerList);
    
    tgui::ChatBox::Ptr RoomChatBox = m_Theme->load("ChatBox");
    RoomChatBox->setSize(m_MainGUI->getSize().x/3, 240);
    RoomChatBox->setPosition(0, 400);
    m_MainGUI->add(RoomChatBox, "GameLobby_RoomChat");
    l_SceneWidgets.push_back(RoomChatBox);
    
    
    return  l_SceneWidgets;
}

std::vector<std::shared_ptr<tgui::Widget>> GameState::MakeMainGameScene()
{
    std::vector<std::shared_ptr<tgui::Widget>> l_SceneWidgets;
    
    return  l_SceneWidgets;
}

std::vector<std::shared_ptr<tgui::Widget>> GameState::MakeEndGameScene()
{
    std::vector<std::shared_ptr<tgui::Widget>> l_SceneWidgets;
    return  l_SceneWidgets;
}

void GameState::StartGame()
{
    HideScene(GUI_SCENE::ROOMLOBBY);
    ShowScene(GUI_SCENE::MAINGAME);
    
    // Start normal render: players, Bases, 'platforms'? , collectables
    m_InMenu = false;
}

void GameState::ReadyUp()
{
    if(NETWORK_MANAGER.GetReadyStatus() == "Not Ready")
        NETWORK_MANAGER.SendTCPMessage("Ready:" + NETWORK_MANAGER.GetRoomName() + ":" + NETWORK_MANAGER.GetUsername() + ":Ready:");
    else
        NETWORK_MANAGER.SendTCPMessage("Ready:" + NETWORK_MANAGER.GetRoomName() + ":" + NETWORK_MANAGER.GetUsername() + ":Not Ready:");
}

void GameState::ExitToMainLobby()
{
    NETWORK_MANAGER.SendTCPMessage("LeaveRoom:" + NETWORK_MANAGER.GetRoomName() + ":" + NETWORK_MANAGER.GetUsername() +":");
}

void GameState::ReturnToGameLobby()
{
    
}












