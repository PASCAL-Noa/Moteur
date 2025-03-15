#pragma once
#include "../ECS.h"
#include <DirectXMath.h>

struct LightComponent : public Component
{
    enum LightType {
        PointLight,
        SpotLight,
        DirectionalLight
    };

    DirectX::XMFLOAT3 Position;
    DirectX::XMFLOAT3 Direction = { 0.0f, -1.0f, 0.0f }; 
    DirectX::XMFLOAT4 Color;
    DirectX::XMFLOAT3 Strength;

    float FalloffStart = 10.0f;
    float FalloffEnd = 10.0f;
    float SpotPower = 64.0f;

    float mSunTheta;
    float mSunPhi;
    LightType mLightType;
    int lightIndex;
};

