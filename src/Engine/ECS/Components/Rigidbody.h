#pragma once
#include "Tools/Utils.h"
#include "../ECS.h"

struct Rigidbody : Component
{
    Utils::Vector3f velocity;
    
    Rigidbody() : velocity{0.0f, 0.0f, 0.0f} {}
            
};
