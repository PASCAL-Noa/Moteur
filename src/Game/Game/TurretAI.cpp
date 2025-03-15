#include "pch.h"
#include "TurretAI.h"
#include "../../Engine/ECS/ECS.h"
#include "../../Engine/ECS/Components/Rigidbody.h"
#include "../../Engine/ECS/Components/MeshRenderer.h"
#include "../../Engine/ECS/Systems/RenderSystem.h"
#include "../../Engine/ECS/Components/Collider.h"
#include "PlayerController.h"

class PlayerController;
constexpr float TURRET_FIRE_RATE = 2.0f;
constexpr float PROJECTILE_SPEED = 10.0f;

void TurretAI::Init()
{
    mScriptManager->AddComponent(*mEntity, new MeshRenderer());
    mScriptManager->GetComponent<MeshRenderer>(*mEntity).textureIndex = 1;
    mScriptManager->GetSystem<RenderSystem>().AddMesh(mEntity, "Turret", MeshProperties::CreateBox(1.0f));
    mScriptManager->AddComponent(*mEntity, new Collider());
    mScriptManager->GetComponent<Collider>(*mEntity).size = Utils::Vector3f(1.0f, 1.0f, 1.0f);
}

void TurretAI::Update(float deltaTime) {
    fireTimer += deltaTime;
    if (fireTimer >= TURRET_FIRE_RATE) {
        FireProjectile();
        fireTimer = 0.0f;
    }
}

void TurretAI::FireProjectile() {
    Entity* projectile = &mScriptManager->CreateEntity();
    mScriptManager->AddComponent(*projectile, new MeshRenderer());
    mScriptManager->GetComponent<MeshRenderer>(*projectile).textureIndex = 3;
    mScriptManager->GetSystem<RenderSystem>().AddMesh(projectile, "EnemyProjectile", MeshProperties::CreateBox(0.2f));
    mScriptManager->AddComponent(*projectile, new Rigidbody());
    mScriptManager->GetComponent<Rigidbody>(*projectile).velocity = { 1.0f, 1.0f, 1.0f };
    mScriptManager->AddComponent(*projectile, new Collider());
    mScriptManager->GetComponent<Collider>(*projectile).size = { 0.2f, 0.2f, 0.2f };

    projectile->transform.SetPosition(mEntity->transform.GetPosition().x, 0.0f, mEntity->transform.GetPosition().z);

    Entity* player = mScriptManager->FindEntityWithScript<PlayerController>();
    if (player) {
        DirectX::XMFLOAT3 direction = player->transform.GetPosition();
        direction.x -= mEntity->transform.GetPosition().x;
        direction.z -= mEntity->transform.GetPosition().z;
        direction = Utils::Normalize(direction);

        mScriptManager->GetComponent<Rigidbody>(*projectile).velocity = { direction.x * PROJECTILE_SPEED, 0.0f, direction.z * PROJECTILE_SPEED };
    }
}
