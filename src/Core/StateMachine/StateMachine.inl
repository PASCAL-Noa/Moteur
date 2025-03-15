template<typename T>
StateMachine<T>::StateMachine(T* owner, int stateCount)
{
    mOwner = owner;
    mCurrentState = -1;
    mBehaviours.resize(stateCount);
}

template<typename T>
StateMachine<T>::~StateMachine()
{
    for (StateBehaviour<T>* pBehaviour : mBehaviours)
        delete pBehaviour;
}

template<typename T>
void StateMachine<T>::SetState(const int state)
{
    if(mCurrentState >= 0 && mCurrentState < mBehaviours.size())
        mBehaviours[mCurrentState]->End();

    mCurrentState = state;

    mBehaviours[mCurrentState]->Start();
}

template<typename T>
void StateMachine<T>::Update()
{
    if(mCurrentState == -1)
        return;

    const int transitionState = mBehaviours[mCurrentState]->Update();

    if (transitionState == -1)
        return;

    SetState(transitionState);
}

template<typename T>
StateBehaviour<T>* StateMachine<T>::CreateBehaviour(int state)
{
    assert(state >= 0 && state < mBehaviours.size());

    StateBehaviour<T>* pBehaviour = new StateBehaviour<T>(mOwner);
    mBehaviours[state] = pBehaviour;

    return pBehaviour;
}