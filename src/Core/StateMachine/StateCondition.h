#pragma once

template <typename T>
class StateTransition;

template<typename T>
class StateCondition
{
    bool Test(T* owner);

public:
    virtual ~StateCondition() = default;

    bool expected = true;

    virtual bool OnTest(T* owner) = 0;

    friend StateTransition<T>;
};

#include "StateCondition.inl"