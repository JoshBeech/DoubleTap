//
//  State.cpp
//  DoubleTap
//
//  Created by BEECH, JOSHUA on 11/12/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#include "State.hpp"

void State::HideScene(GUI_SCENE p_SceneToHide)
{
    for(auto& l_widget: m_GUIScenes[p_SceneToHide])
    {
        l_widget->hide();
        
    }

}

void State::ShowScene(GUI_SCENE p_SceneToShow)
{
    for(auto& l_widget: m_GUIScenes[p_SceneToShow])
        l_widget->show();
}
