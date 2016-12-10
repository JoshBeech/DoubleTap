//
//  Player.cpp
//  DoubleTap
//
//  Created by BEECH, JOSHUA on 22/11/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#include "Player.hpp"

Player::Player(int start_x, int start_y)
{
    m_position.x = start_x;
    m_position.y = start_y;
    
    m_avatar.setRadius(10.f);
    m_avatar.setFillColor(sf::Color::Magenta);
}
