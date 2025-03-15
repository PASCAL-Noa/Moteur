#include "pch.h"

#include "../../Engine/ECS/Components/CameraComponent.h"
#include "Utils.h"

void Utils::PrintMatrix(const DirectX::XMFLOAT4X4& matrix)
{
    std::cout << "----- Matrice Transform -----\n";
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            std::cout << matrix.m[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "----------------------------\n";
}


void Utils::PrintVector(const char* name, const DirectX::XMFLOAT3& vec)
{
    std::cout << name << ": (" << vec.x << ", " << vec.y << ", " << vec.z << ")\n";
}


DirectX::XMFLOAT3 Utils::Normalize(const DirectX::XMFLOAT3& vec)
{
    DirectX::XMVECTOR v = DirectX::XMLoadFloat3(&vec);
    v = DirectX::XMVector3Normalize(v);
    
    DirectX::XMFLOAT3 normalizedVec;
    DirectX::XMStoreFloat3(&normalizedVec, v);
    return normalizedVec;
}

float Utils::Lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

std::wstring Utils::ConvertToWideString(const std::string& str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size_needed);
    return wstr;
}

void Utils::ScreenToWorldRay(const DirectX::XMFLOAT2& screenPos, const CameraComponent& camera,
                             DirectX::XMFLOAT3& rayOrigin, DirectX::XMFLOAT3& rayDirection) {
    float screenWidth = 1920.0f;
    float screenHeight = 1080.0f;

    // Normalisation des coordonnées (écran) (-1 à 1)
    float x = (2.0f * screenPos.x) / screenWidth - 1.0f;
    float y = 1.0f - (2.0f * screenPos.y) / screenHeight;

    DirectX::XMMATRIX projection = DirectX::XMLoadFloat4x4(&camera.mProj);
    DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(&camera.mView);
    DirectX::XMMATRIX invView = DirectX::XMMatrixInverse(nullptr, view);
    DirectX::XMMATRIX invProj = DirectX::XMMatrixInverse(nullptr, projection);

    // Création du rayon en espace caméra
    DirectX::XMVECTOR rayClip = DirectX::XMVectorSet(x, y, 1.0f, 1.0f);
    DirectX::XMVECTOR rayEye = DirectX::XMVector3TransformCoord(rayClip, invProj);
    rayEye = DirectX::XMVectorSet(rayEye.m128_f32[0], rayEye.m128_f32[1], 1.0f, 0.0f);

    // Transformation du rayon en espace monde
    DirectX::XMVECTOR rayWorld = DirectX::XMVector3TransformNormal(rayEye, invView);
    rayWorld = DirectX::XMVector3Normalize(rayWorld);

    DirectX::XMFLOAT3 camPos = camera.GetPosition();

    DirectX::XMStoreFloat3(&rayOrigin, DirectX::XMLoadFloat3(&camPos));
    DirectX::XMStoreFloat3(&rayDirection, rayWorld);
}

