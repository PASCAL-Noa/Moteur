#pragma once

template<typename T>
StateBehaviour<T>::StateBehaviour(T* owner)
{
    mOwner = owner;
}

template<typename T>
StateBehaviour<T>::~StateBehaviour()
{
    for (auto it : mActions)
    {
        delete it;
    }

    for (auto it : mTransitions)
    {
        delete it;
    }
}

template<typename T>
StateTransition<T>* StateBehaviour<T>::CreateTransition(int state)
{
    StateTransition<T>* pTransition = new StateTransition<T>(state);
    mTransitions.push_back(pTransition);

    return pTransition;
}

template<typename T>
void StateBehaviour<T>::AddAction(Action<T>* pAction)
{
    mActions.push_back(pAction);
}

template<typename T>
void StateBehaviour<T>::Start()
{
    for (const auto& a : mActions)
    {
        a->Start(mOwner);
    }
}

template<typename T>
int StateBehaviour<T>::Update()
{
    for (const auto& a : mActions)
    {
        a->Update(mOwner);
    }

    for (const auto& t : mTransitions)
    {
        if (t->Try(mOwner))
        {
            return t->GetTransitionState();
        }
    }

    return -1;
}

template<typename T>
void StateBehaviour<T>::End()
{
    for (const auto& a : mActions)
    {
        a->End(mOwner);
    }
}