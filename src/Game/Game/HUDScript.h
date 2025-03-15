#pragma once
#include "../../Engine/ECS/ECS.h"

class HUDScript : public Script
{
public:
    void Init() override;
    void Update(float deltaTime) override;

    void Render();
    void SetPlayerHealth(float health);
    void SetShieldStatus(bool active);
    void SetBoostLevel(float level);

private:
    float playerHealth = 100.0f;
    bool shieldActive = false;
    float boostLevel = 1.0f;
};
