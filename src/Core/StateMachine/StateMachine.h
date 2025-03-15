#pragma once

#include "StateBehaviour.h"

template<typename T>
class StateMachine
{
    std::vector<StateBehaviour<T>*> mBehaviours;
    int mCurrentState;
    T* mOwner;

public:
    StateMachine(T* owner, int stateCount);
    ~StateMachine();

    void Update();
    void SetState(int state);
    [[nodiscard]] int GetCurrentState() const { return mCurrentState; }

    StateBehaviour<T>* CreateBehaviour(int state);
};

#include "StateMachine.inl"