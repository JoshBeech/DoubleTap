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
        
        if(l_widget->getWidgetType() == "EditBox")
        {
            std::shared_ptr<tgui::EditBox> l_TempEditBox = std::dynamic_pointer_cast<tgui::EditBox>(l_widget);
            
            l_TempEditBox->setText("");
        }
        else if(l_widget->getWidgetType() == "ListBox")
        {
            std::shared_ptr<tgui::ListBox> l_TempListBox = std::dynamic_pointer_cast<tgui::ListBox>(l_widget);
            
            l_TempListBox->deselectItem();
        }
    }
    // Set to default?

}

void State::ShowScene(GUI_SCENE p_SceneToShow)
{
    for(auto& l_widget: m_GUIScenes[p_SceneToShow])
        l_widget->show();
}

std::vector<std::string> State::SplitString(std::string p_String, char p_Delimiter)
{
    std::vector<std::string> l_StringVector;
    
    std::string l_Part;
    std::stringstream l_Stream;
    l_Stream.str(p_String);
    
    while (std::getline(l_Stream, l_Part, p_Delimiter))
    {
        l_StringVector.push_back(l_Part);
    }
    
    return l_StringVector;
}
