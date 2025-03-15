#pragma once
#include "../ECS.h"
#include "Tools/Utils.h"

struct SphereCollider : Component
{
    float radius;
    Utils::Vector3f offset;  

    SphereCollider(float r = 1.0f, const Utils::Vector3f& off = Utils::Vector3f(0.f, 0.f, 0.f)) : radius(r), offset(off) {}
       
};
