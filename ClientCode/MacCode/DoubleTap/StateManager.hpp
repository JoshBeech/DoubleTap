//
//  StateManager.hpp
//  DoubleTap
//
//  Created by BEECH, JOSHUA on 10/12/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#pragma once

//#include <stdio.h>
#include "State.hpp"
#include <string>
#include <map>
#include <stdexcept>

class StateManager
{
public:
    static StateManager& GetInstance();
    ~StateManager() {};
    
    void AddState(std::shared_ptr<State> NewState);
    void ChangeState(int p_StateID);
    std::shared_ptr<State> GetCurrentState() {return m_StateMap.at(m_CurrentStateID); }
    
protected:
    StateManager() { m_CurrentStateID = 0; }
    
private:
    static StateManager* m_instance;
    int m_CurrentStateID;
    std::map<int, std::shared_ptr<State>> m_StateMap;
};

#define STATE_MANAGER StateManager::GetInstance()
