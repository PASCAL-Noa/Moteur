#pragma once
#include "../../Engine/ECS/ECS.h"

class MeteorObstacleAI : public Script {
public:
    void Init() override;
    void Update(float deltaTime) override;


private:
    Utils::Vector3f velocity;
};
