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
    Player();
    sf::RectangleShape& GetAvatar(){return m_avatar;}
    //Get Velo and Pos
    
    void Move(){m_Position += m_Velocity; m_avatar.setPosition(m_Position);}
    void SetPosition(sf::Vector2f new_position){ m_Position = new_position; m_avatar.setPosition(m_Position); }
    sf::Vector2f& GetPosition() {return m_Position;}
    void SetVelocity(sf::Vector2f p_Velocity) {m_Velocity = p_Velocity;}
    sf::Vector2f& GetVelocity() {return m_Velocity;}
private:
    sf::RectangleShape m_avatar;
    sf::Vector2f m_Position;
    sf::Vector2f m_Velocity;
};

#endif /* Player_hpp */
