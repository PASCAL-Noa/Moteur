#pragma once
#include "../ECS.h"
#include "../Components/Collider.h"
#include "../Components/AABBCollider.h"
#include "../Components/SphereCollider.h"

class CollisionSystem : public System
{
public:
    CollisionSystem(Manager* m);

    static AABB ComputeAABB(const Entity* entity, const Collider& collider);
    static bool IntersectAABB(const AABB& a, const AABB& b);
    static bool SphereIntersect(const SphereCollider& sphere1, const DirectX::XMFLOAT3& pos1, const SphereCollider& sphere2, const DirectX::XMFLOAT3& pos2);
    
    void Update(float deltaTime) override;
    void Init() override{}
    
private:
    Manager* mManager;

    static bool IsNeighbor(Entity* a, Entity*b);
    int cellSize = 10;
    
};