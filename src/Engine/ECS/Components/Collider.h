#pragma once
#include "../ECS.h"
#include "Tools/Utils.h"

struct Collider : Component
{
    Utils::Vector3f size;   
    Utils::Vector3f offset; 
    
    Collider() : size(1.0f, 1.0f, 1.0f), offset(0.0f, 0.0f, 0.0f) {}
};