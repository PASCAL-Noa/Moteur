#pragma once

#include "../ECS.h"

struct AABB : Component
{
    Utils::Vector3f min;  
    Utils::Vector3f max;  
    
    AABB() : min{0.0f, 0.0f, 0.0f}, max{0.0f, 0.0f, 0.0f}{}
};
