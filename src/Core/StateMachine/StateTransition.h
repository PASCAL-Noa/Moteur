#pragma once

#include "StateCondition.h"

template<typename T>
class StateTransition
{
protected:
    std::vector<StateCondition<T>*> mConditions;
    int mTransitionState;

public:
    explicit StateTransition(const int transitionState) : mTransitionState(transitionState) {}

    template<typename U>
    U* AddCondition();

    bool Try(T* owner);
    int GetTransitionState() const { return mTransitionState; }
};

#include "StateTransition.inl"