//
//  MenuState.hpp
//  DoubleTap
//
//  Created by BEECH, JOSHUA on 10/12/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#ifndef MenuState_hpp
#define MenuState_hpp

#include "State.hpp"
#include <iostream>

// For main menu and lobby
class MenuState : public State
{
public:
    MenuState();
    ~MenuState();
    void Create() override;
    void Destroy() override;
    void Enter() override;
    void Exit() override;
    void Update() override;
    void Render() override;
private:
    // GUI Items
};

#endif /* MenuState_hpp */
