//
//  StateMachine.cpp
//  DoubleTap
//
//  Created by BEECH, JOSHUA on 10/12/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#include "StateManager.hpp"

StateManager *StateManager::m_instance = NULL;

StateManager& StateManager::GetInstance()
{
    if(NULL == m_instance)
    {
        m_instance = new StateManager;
    }
    
    return *m_instance;
}

void StateManager::AddState(std::shared_ptr<State> NewState)
{
    if(m_StateMap.find(NewState->GetID()) != m_StateMap.end())
    {
        throw std::invalid_argument("State already exists");
    }
    else
        m_StateMap.insert(std::pair<StateID, std::shared_ptr<State>>(NewState->GetID(), NewState));
    
    if(m_StateMap.size() == 1)
        m_CurrentStateID = NewState->GetID();
}

void StateManager::ChangeState(StateID NextState)
{
    // Call current state exit and next state enter
    m_StateMap.at(m_CurrentStateID)->Exit();
    m_StateMap.at(NextState)->Enter();
    m_CurrentStateID = NextState;
}
