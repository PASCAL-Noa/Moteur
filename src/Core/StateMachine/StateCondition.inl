#pragma once

template<typename T>
bool StateCondition<T>::Test(T* owner)
{
    return expected == OnTest(owner);
}
