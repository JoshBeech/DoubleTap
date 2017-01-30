//
//  Bullet.hpp
//  DoubleTap
//
//  Created by BEECH, JOSHUA on 23/01/2017.
//  Copyright © 2017 Cordry, Julien. All rights reserved.
//

#ifndef Bullet_hpp
#define Bullet_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>

class Bullet
{
public:
    Bullet(sf::Vector2f p_StartPosition, sf::Vector2f p_Velocity);
    sf::CircleShape& GetAvatar() {return m_Avatar;}
    void Move(){m_Position += m_Velocity; m_Avatar.setPosition(m_Position);}
private:
    sf::CircleShape m_Avatar;
    sf::Vector2f m_Position;
    sf::Vector2f m_Velocity;
};



#endif /* Bullet_hpp */
