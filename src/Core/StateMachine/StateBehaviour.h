#pragma once

#include "Action.h"
#include "StateTransition.h"

template<typename T>
class StateBehaviour
{
    std::vector<Action<T>*> mActions;
    std::vector<StateTransition<T>*> mTransitions;
    T* mOwner;

public:
    StateBehaviour(T* owner);
    ~StateBehaviour();

    void Start();
    int Update();
    void End();

    void AddAction(Action<T>* action);
    StateTransition<T>* CreateTransition(int state);
};

#include "StateBehaviour.inl"