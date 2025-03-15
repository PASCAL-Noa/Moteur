#pragma once

#include "../ECS.h"

class PhysicsSystem : public System
{
public:
    explicit PhysicsSystem(Manager* manager) : mManager(manager) {}
    
    void Update(float deltaTime) override;

    void Init() override {}
    
private:
    Manager* mManager;  
};
