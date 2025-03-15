#include "pch.h"
#include "CollisionSystem.h"

CollisionSystem::CollisionSystem(Manager* m)
    : mManager(m) {
}


AABB CollisionSystem::ComputeAABB(const Entity* entity, const Collider& collider)
{
    DirectX::XMFLOAT3 pos = entity->transform.GetPosition();
    pos.x += collider.offset.x;
    pos.y += collider.offset.y;
    pos.z += collider.offset.z;
        
    AABB aabb;
    aabb.min = { pos.x - collider.size.x * 0.5f, 
                 pos.y - collider.size.y * 0.5f, 
                 pos.z - collider.size.z * 0.5f };
    aabb.max = { pos.x + collider.size.x * 0.5f, 
                 pos.y + collider.size.y * 0.5f, 
                 pos.z + collider.size.z * 0.5f };
    return aabb;
}

bool CollisionSystem::IntersectAABB(const AABB& a, const AABB& b)
{
    return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
           (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
           (a.min.z <= b.max.z && a.max.z >= b.min.z);
}

bool CollisionSystem::SphereIntersect(const SphereCollider& sphere1, const DirectX::XMFLOAT3& pos1, const SphereCollider& sphere2, const DirectX::XMFLOAT3& pos2)
{
    // Calcul des centres des sphères 
    float center1X = pos1.x + sphere1.offset.x;
    float center1Y = pos1.y + sphere1.offset.y;
    float center1Z = pos1.z + sphere1.offset.z;

    float center2X = pos2.x + sphere2.offset.x;
    float center2Y = pos2.y + sphere2.offset.y;
    float center2Z = pos2.z + sphere2.offset.z;

    // Calcul de la distance au carré entre les centres
    float dx = center1X - center2X;
    float dy = center1Y - center2Y;
    float dz = center1Z - center2Z;
    float distanceSquared = dx * dx + dy * dy + dz * dz;

    // La somme des rayons des deux sphères
    float radiusSum = sphere1.radius + sphere2.radius;

    return distanceSquared <= (radiusSum * radiusSum);
}

void CollisionSystem::Update(float deltaTime)
{
    auto& entities = mManager->GetEntities();
    
    for (Entity* entity : entities)
    {
        entity->UpdateGrid(cellSize);
    }
    
    for (size_t i = 0; i < entities.size(); i++)
    {
        for (size_t j = i + 1; j < entities.size(); j++)
        {
            if (IsNeighbor(entities[i], entities[j]) == false)
                continue;
            
            bool collisionDetected = false;
            
            if (mManager->HasComponent<SphereCollider>(*entities[i]) && mManager->HasComponent<SphereCollider>(*entities[j]))
            {
                SphereCollider& sphere1 = mManager->GetComponent<SphereCollider>(*entities[i]);
                SphereCollider& sphere2 = mManager->GetComponent<SphereCollider>(*entities[j]);
                DirectX::XMFLOAT3 pos1 = entities[i]->transform.GetPosition();
                DirectX::XMFLOAT3 pos2 = entities[j]->transform.GetPosition();
                collisionDetected = SphereIntersect(sphere1, pos1, sphere2, pos2);
            }
            else if (mManager->HasComponent<Collider>(*entities[i]) && mManager->HasComponent<Collider>(*entities[j]))
            {
                AABB aabb1 = ComputeAABB(entities[i], mManager->GetComponent<Collider>(*entities[i]));
                AABB aabb2 = ComputeAABB(entities[j], mManager->GetComponent<Collider>(*entities[j]));
                collisionDetected = IntersectAABB(aabb1, aabb2);
            }

            if (collisionDetected)
            {
                for (Script* script : entities[i]->GetScripts())
                {
                    script->OnCollide(entities[j]);
                }
                for (Script* script : entities[j]->GetScripts())
                {
                    script->OnCollide(entities[i]);
                }
            }
        }
    }
}

bool CollisionSystem::IsNeighbor(Entity* a, Entity* b)
{
    if (b->gridPosition.x < a->gridPosition.x -1 || b->gridPosition.x > a->gridPosition.x +1)
    {
        return false;
    }
    
    if (b->gridPosition.y < a->gridPosition.y -1 || b->gridPosition.y > a->gridPosition.y +1)
    {
        return false;
    }

    if (b->gridPosition.z < a->gridPosition.z -1 || b->gridPosition.z > a->gridPosition.z +1)
    {
        return false;
    }
    
    return true;
}