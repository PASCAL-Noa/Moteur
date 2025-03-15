#pragma once
#include "../../Engine/ECS/ECS.h"

class SpawnScript
{
public:
    static void SpawnMeteor(Manager& manager);
    static void SpawnHomingMeteor(Manager& manager);
    static void SpawnTurret(Manager& manager);
};
