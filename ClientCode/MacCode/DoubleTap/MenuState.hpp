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
    void Render() override;
    
    void LoadScenes() override;
    std::vector<std::shared_ptr<tgui::Widget>> MakeLoginScene();
    std::vector<std::shared_ptr<tgui::Widget>> MakeLobbyScene();
    
    void Login();
private:
    // GUI Items
};

#endif /* MenuState_hpp */
