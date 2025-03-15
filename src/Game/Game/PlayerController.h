#pragma once
#include "../../Engine/ECS/ECS.h"
#include "../../Engine/ECS/Systems/InputSystem.h"

class PlayerController : public Script {
public:
    void Init() override;
    void Update(float deltaTime) override;


private:
    void HandleMovement(float deltaTime);
    void HandleBoost(float deltaTime);
    void HandleShooting();
    void HandleShield(float deltaTime);
    void EnforceBoundaries();

    DirectX::XMFLOAT3 GetMouseWorldPosition();

    InputSystem* inputSystem;
    float speed = 5.0f;
    float boostMultiplier = 2.0f;
    bool isBoosting = false;
    bool shieldActive = false;
    float shieldTimer = 0.0f;

    const float minX = -10.0f, maxX = 10.0f;
    const float minY = -5.0f, maxY = 5.0f;
};
