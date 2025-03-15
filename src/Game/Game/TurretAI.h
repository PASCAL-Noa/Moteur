#pragma once
#include "../../Engine/ECS/ECS.h"

class TurretAI : public Script {
public:
    void Init() override;
    void Update(float deltaTime) override;
    void FireProjectile();


private:
    float fireTimer = 0.0f;
};
