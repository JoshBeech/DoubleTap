//
//  InputManager.cpp
//  DoubleTap
//
//  Created by BEECH, JOSHUA on 10/12/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#include "InputManager.hpp"

InputManager::InputManager()
{
    SetupTrackedKeys();
    SetupTrackedMouseButton();
}

void InputManager::SetupTrackedKeys()
{
    m_TrackedKeys.push_back(sf::Keyboard::Key::Up);
    m_TrackedKeys.push_back(sf::Keyboard::Key::Down);
    m_TrackedKeys.push_back(sf::Keyboard::Key::Left);
    m_TrackedKeys.push_back(sf::Keyboard::Key::Right);
    
    m_TrackedKeys.push_back(sf::Keyboard::Key::W);
    m_TrackedKeys.push_back(sf::Keyboard::Key::S);
    m_TrackedKeys.push_back(sf::Keyboard::Key::A);
    m_TrackedKeys.push_back(sf::Keyboard::Key::D);
    
    m_TrackedKeys.push_back(sf::Keyboard::Key::Space);
    m_TrackedKeys.push_back(sf::Keyboard::Key::Escape);
}

void InputManager::AddKey(sf::Keyboard::Key KeyToTrack)
{
    m_TrackedKeys.push_back(KeyToTrack);
}

void InputManager::SetupTrackedMouseButton()
{
    m_TrackedMouseButton.push_back(sf::Mouse::Button::Left);
}

void InputManager::AddMouseButton(sf::Mouse::Button ButtonToTrack)
{
    m_TrackedMouseButton.push_back(ButtonToTrack);
}

bool InputManager::IsEventTracked(sf::Event CurrentEvent)
{
    // Search Tracked Keys - Release?
    if(CurrentEvent.type == sf::Event::KeyPressed)
    {
        std::vector<sf::Keyboard::Key>::iterator KeyIterator;
        
        KeyIterator = std::find(m_TrackedKeys.begin(), m_TrackedKeys.end(), CurrentEvent.key.code);
        
        if(KeyIterator != m_TrackedKeys.end())
            return  true;
        else
            return false;
    }
    // Search Tracked Mouse buttons
    else if (CurrentEvent.type == sf::Event::MouseButtonPressed)
    {
        std::vector<sf::Mouse::Button>::iterator MouseIterator;
        
        MouseIterator = std::find(m_TrackedMouseButton.begin(), m_TrackedMouseButton.end(), CurrentEvent.mouseButton.button);
        
        if(MouseIterator != m_TrackedMouseButton.end())
            return  true;
        else
            return false;
    }
    else
        return false;
}
