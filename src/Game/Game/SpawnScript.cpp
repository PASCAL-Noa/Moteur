#include "pch.h"
#include "SpawnScript.h"
#include "MeteorHomingAI.h"
#include "MeteorObstacleAI.h"
#include "TurretAI.h"

void SpawnScript::SpawnMeteor(Manager& manager)
{
    Entity& meteor = manager.CreateEntity();
    manager.AttachScript(&meteor, new MeteorObstacleAI());
    std::cout << "[DEBUG] Spawned a Meteor at (10.0f, 0.0f)" << std::endl;
}

void SpawnScript::SpawnHomingMeteor(Manager& manager)
{
    Entity& meteor = manager.CreateEntity();
    manager.AttachScript(&meteor, new MeteorHomingAI());

    float randomX = (rand() % 20) - 10.0f;
    meteor.transform.SetPosition(randomX, 10.0f, 0.0f);
    std::cout << "[DEBUG] Spawned a Homing Meteor at (" << randomX << ", 10.0f, 0.0f)" << std::endl;
}

void SpawnScript::SpawnTurret(Manager& manager)
{
    Entity& turret = manager.CreateEntity();
    manager.AttachScript(&turret, new TurretAI());

    float randomX = (rand() % 20) - 10.0f;
    turret.transform.SetPosition(randomX, 10.0f, 0.0f);
    std::cout << "[DEBUG] Spawned a Turret at (" << randomX << ", 10.0f, 0.0f)" << std::endl;
}
