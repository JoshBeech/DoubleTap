//
//  Player.cpp
//  DoubleTap
//
//  Created by BEECH, JOSHUA on 22/11/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#include "Player.hpp"

Player::Player()
{
    m_Position.x = 0;
    m_Position.y = 0;
    
    m_avatar.setSize(sf::Vector2f(25,25));
    m_avatar.setFillColor(sf::Color::Cyan);
}
