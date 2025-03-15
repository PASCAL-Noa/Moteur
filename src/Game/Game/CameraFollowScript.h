#pragma once
#include "../../Engine/ECS/ECS.h"

class CameraFollowScript : public Script
{
public:
    void Init() override;
    void Update(float deltaTime) override;
    void Update(float deltaTime, Manager& manager);

    void SetTarget(Entity* player);

private:
    Entity* target = nullptr;
};
