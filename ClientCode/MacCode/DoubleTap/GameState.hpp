//
//  GameState.hpp
//  DoubleTap
//
//  Created by BEECH, JOSHUA on 11/12/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#ifndef GameState_hpp
#define GameState_hpp

#include "State.hpp"
#include <iostream>

class GameState : public State
{
public:
    GameState();
    ~GameState();
    void Create() override;
    void Destroy() override;
    void Enter() override;
    void Exit() override;
    void Update() override;
    void Render() override;
private:
    // GUI Items
};

#endif /* GameState_hpp */
