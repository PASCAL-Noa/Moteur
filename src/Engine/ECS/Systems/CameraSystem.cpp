#include "pch.h"
#include "../Components/CameraComponent.h"
#include "../Components/InputComponent.h"
#include "../Core/Tools/Transform.h"
#include "../Core/Tools/Utils.h"
#include "InputSystem.h"
#include "CameraSystem.h"

CameraSystem::CameraSystem(Manager* m)
    : manager(m) {
}

void CameraSystem::Update(float dt)
{
    for (auto& entity : manager->GetEntities()) // Remplacé `manager.` par `manager->`
    {
        if (!entity || !entity->IsAlive()) return;
        if (manager->HasComponent<InputComponent>(*entity) && manager->HasComponent<CameraComponent>(*entity)) {
            auto& inputSystem = manager->GetSystem<InputSystem>();
            auto& inputComponent = manager->GetComponent<InputComponent>(*entity);
            auto& cameraComponent = manager->GetComponent<CameraComponent>(*entity);

            static int lastMouseX = inputComponent.mMouseX;
            static int lastMouseY = inputComponent.mMouseY;

            if (inputSystem.GetMouseButtonState(VK_RBUTTON) == KeyState::DOWN) {
                int currentMouseX = inputComponent.mMouseX;
                int currentMouseY = inputComponent.mMouseY;

                float deltaX = static_cast<float>(currentMouseX - lastMouseX);
                float deltaY = static_cast<float>(currentMouseY - lastMouseY);

                cameraComponent.mCameraTransform.SetTheta(cameraComponent.mCameraTransform.GetTheta() + deltaX * 0.005f);
                cameraComponent.mCameraTransform.SetPhi(cameraComponent.mCameraTransform.GetPhi() + deltaY * 0.005f);

                lastMouseX = currentMouseX;
                lastMouseY = currentMouseY;
            }
            else {
                lastMouseX = inputComponent.mMouseX;
                lastMouseY = inputComponent.mMouseY;
            }

            float moveSpeed = 0.2f;
            DirectX::XMFLOAT3 position = entity->transform.GetPosition();

            if (inputSystem.GetKeyState('Z') == KeyState::DOWN) {
                DirectX::XMFLOAT3 forward = cameraComponent.mCameraTransform.GetForward();
                position.z += moveSpeed * forward.z;
            }
            if (inputSystem.GetKeyState('S') == KeyState::DOWN) {
                DirectX::XMFLOAT3 forward = cameraComponent.mCameraTransform.GetForward();
                position.z -= moveSpeed * forward.z;
            }
            if (inputSystem.GetKeyState('Q') == KeyState::DOWN) {
                DirectX::XMFLOAT3 right = cameraComponent.mCameraTransform.GetRight();
                position.x -= moveSpeed * right.x;
            }
            if (inputSystem.GetKeyState('D') == KeyState::DOWN) {
                DirectX::XMFLOAT3 right = cameraComponent.mCameraTransform.GetRight();
                position.x += moveSpeed * right.x;
            }
            if (inputSystem.GetKeyState(VK_LSHIFT) == KeyState::DOWN) {
                mFov = Utils::Lerp(mFov, mSpeedFov, 5.0f * dt);
                mSpeed = Utils::Lerp(mSpeed, 3.0f, 5.0f * dt);

                manager->GetSystem<CameraSystem>().SetFOV(mFov);
                moveSpeed = Utils::Lerp(moveSpeed, 10000.5f, 5.0f * dt); 
            }
            else {
                mFov = Utils::Lerp(mFov, mBaseFov, 5.0f * dt);
                mSpeed = Utils::Lerp(mSpeed, 1.0f, 5.0f * dt);

                manager->GetSystem<CameraSystem>().SetFOV(mFov);
                moveSpeed = Utils::Lerp(moveSpeed, 10.2f, 5.0f * dt);
            }

            if (inputSystem.GetKeyState('F') == KeyState::DOWN) {
				inputSystem.ToggleCursorLock();
            }


            // Mettre à jour la position de l'entité
            entity->transform.SetPosition(position.x, position.y, position.z);

            // Appliquer la position de l'entité à la caméra
            cameraComponent.mCameraTransform.SetPosition(position.x, position.y, position.z);

            // Créer la rotation quaternion
            DirectX::XMVECTOR rotationQuat = DirectX::XMQuaternionRotationRollPitchYaw(
                cameraComponent.mCameraTransform.GetPhi(),
                cameraComponent.mCameraTransform.GetTheta(),
                0.0f
            );

            cameraComponent.mCameraTransform.RotateQuaternion(DirectX::XMFLOAT4(
                DirectX::XMVectorGetX(rotationQuat),
                DirectX::XMVectorGetY(rotationQuat),
                DirectX::XMVectorGetZ(rotationQuat),
                DirectX::XMVectorGetW(rotationQuat)
            ));

            // Calcul de la matrice de vue
            DirectX::XMMATRIX cView = cameraComponent.mCameraTransform.GetMatrix();
            cView = XMMatrixInverse(nullptr, cView);
            XMStoreFloat4x4(&cameraComponent.mView, cView);
        }
    }
}

void CameraSystem::Init()
{
    for (auto& entity : manager->GetEntities())
    {
        if (manager->HasComponent<InputComponent>(*entity) && manager->HasComponent<CameraComponent>(*entity))
        {
            auto& cameraComponent = manager->GetComponent<CameraComponent>(*entity);
            cameraComponent.mCameraTransform.SetPosition(
                entity->transform.GetPosition().x,
                entity->transform.GetPosition().y,
                entity->transform.GetPosition().z);


			SetFOV(mFov);

            cameraComponent.mCameraTransform.SetPhi(XM_PI / 8);
        }
    }
}

void CameraSystem::SetFOV(float fov)
{
    for (auto& entity : manager->GetEntities())
    {
        if (manager->HasComponent<InputComponent>(*entity) && manager->HasComponent<CameraComponent>(*entity))
        {
            auto& cameraComponent = manager->GetComponent<CameraComponent>(*entity);

            XMMATRIX ProjMatrix = XMMatrixPerspectiveFovLH(fov * (DirectX::XM_PI / 180.0f), 1920.0f / 1080.0f, 0.1f, 20000.0f);
            DirectX::XMFLOAT4X4 dd;
            XMStoreFloat4x4(&dd, ProjMatrix);

            cameraComponent.mProj = dd;
        }
    }
}