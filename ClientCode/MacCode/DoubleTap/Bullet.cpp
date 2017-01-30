//
//  Bullet.cpp
//  DoubleTap
//
//  Created by BEECH, JOSHUA on 23/01/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#include "Bullet.hpp"

Bullet::Bullet(sf::Vector2f p_StartPosition, sf::Vector2f p_Velocity)
{
    m_Position = p_StartPosition;
    m_Velocity = p_Velocity;
    
    m_Avatar.setRadius(5);
    m_Avatar.setFillColor(sf::Color::Red);
}
