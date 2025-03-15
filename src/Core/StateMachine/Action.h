#pragma once

template<typename T>
class Action
{
public:
    virtual ~Action() = default;

    virtual void Start(T* owner) = 0;
    virtual void Update(T* owner) = 0;
    virtual void End(T* owner) = 0;
};