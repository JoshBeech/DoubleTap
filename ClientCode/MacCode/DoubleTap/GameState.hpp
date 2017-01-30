//
//  GameState.hpp
//  DoubleTap
//
//  Created by BEECH, JOSHUA on 11/12/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#ifndef GameState_hpp
#define GameState_hpp

#include <iostream>
#include "State.hpp"
#include "Player.hpp"
#include "Bullet.hpp"

class GameState : public State
{
public:
    GameState(tgui::Gui* p_GUI, std::shared_ptr<tgui::Theme> p_Theme);
    ~GameState();
    void Create(tgui::Gui* p_GUI, std::shared_ptr<tgui::Theme> p_Theme) override;
    void Destroy() override;
    void Enter() override;
    void Exit() override;
    void Update() override;
    void Render(sf::RenderWindow& p_RenderWindow) override;
    void ReceiveInput(sf::Event p_Event) override;
    
    void CheckMessages(int p_Protocol) override;
    
    void LoadScenes() override;
    std::vector<std::shared_ptr<tgui::Widget>> MakeLobbyScene();
    std::vector<std::shared_ptr<tgui::Widget>> MakeMainGameScene();
    std::vector<std::shared_ptr<tgui::Widget>> MakeEndGameScene();
    
    void StartGame();
    void ReadyUp();
    void ExitToMainLobby();
    void ReturnToGameLobby();
    
    Player& GetPlayer() {return m_User;}
private:
    // GUI Items
    // Network = 7, RoomUpdate = 8, Leave = 9, Chat = 10, Game = 11, Position = 12, Collectable = 13, Collision = 14
    enum e_GameStateMessages {NETWORK = 7, ROOMUPDATE = 8, LEAVEROOM, CHAT, GAME, POSITION, COLLECTABLE, COLLISION};
    bool m_InMenu = true;
    Player m_User;
    std::vector<Bullet> m_Bullets;
};

#endif /* GameState_hpp */
