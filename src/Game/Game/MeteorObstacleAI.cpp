#include "pch.h"
#include "MeteorObstacleAI.h"
#include "ECS/ECS.h"
#include "../../Engine/ECS/Components/Rigidbody.h"
#include "../../Engine/ECS/Components/MeshRenderer.h"

#include "PlayerController.h"

class PlayerController;

void MeteorObstacleAI::Init() {

    mScriptManager->AddComponent(*mEntity, new MeshRenderer());
    mScriptManager->GetComponent<MeshRenderer>(*mEntity).textureIndex = 4;
    mScriptManager->GetSystem<RenderSystem>().AddMesh(mEntity, "Meteor", MeshProperties::CreateSphere(1.0f, 16, 16));
    mScriptManager->AddComponent(*mEntity, new Rigidbody());
    mScriptManager->GetComponent<Rigidbody>(*mEntity).velocity = Utils::Vector3f(1.0f, 0.5f, 1.0f);
    mScriptManager->AddComponent(*mEntity, new Collider());
    mScriptManager->GetComponent<Collider>(*mEntity).size = Utils::Vector3f(1.0f, 1.0f, 1.0f);
    
    float randomX = (rand() % 20) - 10.0f;
    mEntity->transform.SetPosition(randomX, 10.0f, 0.0f);
    Entity* player = mScriptManager->FindEntityWithScript<PlayerController>();
    if (!player) return;

    Rigidbody& rb = mScriptManager->GetComponent<Rigidbody>(*mEntity);
    DirectX::XMFLOAT3 meteorPos = mEntity->transform.GetPosition();
    DirectX::XMFLOAT3 playerPos = player->transform.GetPosition();

    DirectX::XMFLOAT3 direction = {
        playerPos.x - meteorPos.x,
        playerPos.y - meteorPos.y,
        playerPos.z - meteorPos.z
    };

    direction = Utils::Normalize(direction);

    float randomOffset = ((rand() % 100) / 100.0f) * 0.5f - 0.25f;
    direction.x += randomOffset;
    direction.z += randomOffset;

    velocity = Utils::Vector3f(direction.x * 5.0f, direction.y * 5.0f, direction.z * 5.0f);

    rb.velocity = velocity;
}

void MeteorObstacleAI::Update(float deltaTime)
{
    //rien
}
