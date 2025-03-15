#pragma once
#include <DirectXMath.h>
#include "../Core/Maths/MathHelper.h"

struct sLight {
    DirectX::XMFLOAT3 Position; // 12 octets
    float padding1;             // 4 octets (alignement sur 16 octets)

    DirectX::XMFLOAT3 Direction = { 0.0f, -1.0f, 0.0f }; // 12 octets
    float padding5;              // 4 octets (alignement sur 16 octets)

    DirectX::XMFLOAT4 Color;     // 16 octets (d�j� align�)

    DirectX::XMFLOAT3 Strength;  // 12 octets
    float FalloffStart = 10.0f;          

    float FalloffEnd = 10.0f;           
    float SpotPower = 64.0f;            
    DirectX::XMFLOAT2 padding2; 
};


struct ObjectConstants
{
    DirectX::XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
    DirectX::XMFLOAT4X4 World;
    DirectX::XMFLOAT4X4 View;
    DirectX::XMFLOAT4X4 Projection;
};

struct PassConstants
{
    DirectX::XMFLOAT4 AmbientColor;
    DirectX::XMFLOAT3 EyePosW;
    float padding;
    sLight light[16];
};

