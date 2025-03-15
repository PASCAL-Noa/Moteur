#pragma once

template<typename T>
template<typename U>
U* StateTransition<T>::AddCondition()
{
    static_assert(std::is_base_of<StateCondition<T>, U>::value, "U must be derived from StateCondition");

    U* pCondition = new U();

    mConditions.push_back(pCondition);

    return pCondition;
}

template<typename T>
bool StateTransition<T>::Try(T* owner)
{
    for (const auto& c : mConditions)
    {
        if (!c->Test(owner))
            return false;
    }

    return true;
}