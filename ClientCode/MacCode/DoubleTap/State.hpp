//
//  State.hpp
//  DoubleTap
//
//  Created by BEECH, JOSHUA on 10/12/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#ifndef State_hpp
#define State_hpp

#include <stdio.h>
#include <TGUI/TGUI.hpp>
#include <map>

enum StateID { MENU, GAME };
enum GUI_SCENE {LOGIN, LOBBY, SETTINGS, MAINGAME, ENDGAME };

// Abstract
class State
{
public:
    virtual void Create(tgui::Gui* p_GUI, std::shared_ptr<tgui::Theme> p_Theme) = 0;
    virtual void Destroy() = 0;
    virtual void Enter() = 0;
    virtual void Exit() = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;
    
    virtual void LoadScenes() = 0;
    void HideScene(GUI_SCENE p_SceneToHide);
    void ShowScene(GUI_SCENE p_SceneToShow);
    StateID GetID(){return m_ID;}
protected:
    StateID m_ID;
    std::shared_ptr<tgui::Theme> m_Theme;
    tgui::Gui* m_MainGUI;
    std::map<GUI_SCENE, std::vector<std::shared_ptr<tgui::Widget>>> m_GUIScenes;
};

#endif /* State_hpp */
