//
//  Player.hpp
//  DoubleTap
//
//  Created by BEECH, JOSHUA on 22/11/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>


class Player
{
public:
    Player(int start_x, int start_y);
    sf::RectangleShape GetAvatar(){return m_avatar;}
    //Get Velo and Pos
    
    void move(){m_position += m_velocity;}
    void SetPosition(sf::Vector2i new_position){ m_position = new_position; }
    void SetVelocity(sf::Vector2i new_velocity){ m_velocity = new_velocity; }
private:
    sf::RectangleShape m_avatar;
    sf::Vector2i m_position;
    sf::Vector2i m_velocity;
};

#endif /* Player_hpp */
