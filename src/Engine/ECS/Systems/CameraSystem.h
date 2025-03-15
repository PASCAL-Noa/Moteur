#pragma once

#include "../ECS.h"
#include <DirectXMath.h>

using namespace DirectX;

class CameraSystem : public System {
private:
    Manager* manager;

public:
    CameraSystem(Manager* m);
    void Update(float dt) override;
    void Init()override;
    void SetFOV(float fov);
private:
    float mFov = 65.f;
    float mBaseFov = mFov;
	float mSpeed;
	float mSpeedFov = 105.f;
};
