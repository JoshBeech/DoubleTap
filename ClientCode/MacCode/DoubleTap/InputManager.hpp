//
//  InputManager.hpp
//  DoubleTap
//
//  Created by BEECH, JOSHUA on 10/12/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#ifndef InputManager_hpp
#define InputManager_hpp

#include <stdio.h>
#include <SFML/Window.hpp>

class InputManager
{
public:
    InputManager();
    void SetupTrackedKeys();
    void AddKey(sf::Keyboard::Key KeyToTrack);
    
    void SetupTrackedMouseButton();
    void AddMouseButton(sf::Mouse::Button ButtonToTrack);
    
    bool IsEventTracked(sf::Event CurrentEvent);
private:
    std::vector<sf::Keyboard::Key> m_TrackedKeys;
    std::vector<sf::Mouse::Button> m_TrackedMouseButton;
};

#endif /* InputManager_hpp */
