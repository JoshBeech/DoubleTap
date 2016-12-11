//
//  State.hpp
//  DoubleTap
//
//  Created by BEECH, JOSHUA on 10/12/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#ifndef State_hpp
#define State_hpp

#include <stdio.h>

enum StateID { MENU, GAME };

// Abstract
class State
{
public:
    virtual void Create() = 0;
    virtual void Destroy() = 0;
    virtual void Enter() = 0;
    virtual void Exit() = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;
    //virtual void GUI - add when ready
    StateID GetID(){return m_ID;}
protected:
    StateID m_ID;
};

#endif /* State_hpp */
