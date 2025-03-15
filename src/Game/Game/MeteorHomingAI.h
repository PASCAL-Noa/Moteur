#pragma once
#include "../../Engine/ECS/ECS.h"

class MeteorHomingAI : public Script {
public:
    void Init() override;
    void Update(float deltaTime) override;

};