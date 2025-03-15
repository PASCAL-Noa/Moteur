#include "pch.h"
#include "CameraFollowScript.h"

#include "../../Core/Tools/Transform.h"

struct CameraComponent;

void CameraFollowScript::Init() {
    std::cout << "[DEBUG] Camera Follow Script Initialized" << std::endl;
}

void CameraFollowScript::Update(float deltaTime, Manager& manager)
{
    if (!target) return;

    Transform& playerTransform = target->transform;
    Transform& cameraTransform = manager.FindEntityWithComponent<CameraComponent>()->transform;

    DirectX::XMFLOAT3 targetPosition = playerTransform.GetPosition();

    DirectX::XMFLOAT3 cameraOffset = { 0.0f, 5.0f, 10.0f };
    cameraTransform.SetPosition(
        targetPosition.x + cameraOffset.x,
        targetPosition.y + cameraOffset.y,
        targetPosition.z + cameraOffset.z
    );
}

void CameraFollowScript::Update(float deltaTime)
{
    // nothing to do here
}

void CameraFollowScript::SetTarget(Entity* player) {
    target = player;
    std::cout << "[DEBUG] Camera target set!" << std::endl;
}