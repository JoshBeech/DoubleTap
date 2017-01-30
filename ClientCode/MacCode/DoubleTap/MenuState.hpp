//
//  MenuState.hpp
//  DoubleTap
//
//  Created by BEECH, JOSHUA on 10/12/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#ifndef MenuState_hpp
#define MenuState_hpp

#include "State.hpp"
#include <iostream>
#include <sstream>
#include <future>

// For main menu and lobby
class MenuState : public State
{
public:
    MenuState(tgui::Gui* p_GUI, std::shared_ptr<tgui::Theme> p_Theme);
    ~MenuState();
    void Create(tgui::Gui* p_GUI, std::shared_ptr<tgui::Theme> p_Theme) override;
    void Destroy() override;
    void Enter() override;
    void Exit() override;
    void Update() override;
    void Render(sf::RenderWindow& p_RenderWindow) override;
    void ReceiveInput(sf::Event p_Event) override;
    
    void CheckMessages(int p_Protocol) override;
    
    void LoadScenes() override;
    std::vector<std::shared_ptr<tgui::Widget>> MakeLoginScene();
    std::vector<std::shared_ptr<tgui::Widget>> MakeLobbyScene();
    void MakeCreatePopUp();
    
    void Login();
    void Register();
    void Logout();
    void CreateRoom();
    void JoinRoom();
    void GetRoomList();
    void RefreshRoomList();
private:
    // Reg = 0, Login = 1, Logout = 2, CreateRoom = 3, JoinRoom = 4, RoomListUpdate = 5, RoomListRefresh = 6
    enum e_MenuStateMessages {REGISTER = 0, LOGIN, LOGOUT, CREATEROOM, JOINROOM, UPDATELIST, REFRESHLIST, NETWORK};
};

#endif /* MenuState_hpp */
