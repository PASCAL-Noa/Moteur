#include "pch.h"
#include "PhysicsSystem.h"
#include "../Components/Rigidbody.h"

void PhysicsSystem::Update(float deltaTime)
{
    for (auto* entity : mManager->GetEntities())
    {
        if (mManager->HasComponent<Rigidbody>(*entity) && entity->IsAlive()) {

            Rigidbody& rb = mManager->GetComponent<Rigidbody>(*entity);

            DirectX::XMFLOAT3 newPosition = entity->transform.GetPosition();

            newPosition.x += rb.velocity.x * deltaTime;
            newPosition.y += rb.velocity.y * deltaTime;
            newPosition.z += rb.velocity.z * deltaTime;

            entity->transform.SetPosition(newPosition.x, newPosition.y, newPosition.z);
            if (entity->transform.GetPosition().y > 200) {
                entity->Destroy();
            }
        }
        
    }
}