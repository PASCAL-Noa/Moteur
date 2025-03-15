#pragma once

#include "../ECS.h"
#include <map>

enum KeyState {
    NONE,
    PUSH,
    DOWN,
    UP
};

struct InputComponent : public Component {
    std::map<int, KeyState> mKeyStates;
    float mMouseX = 0;
    float mMouseY = 0;

    InputComponent() = default;
};
