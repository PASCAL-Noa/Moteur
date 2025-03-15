#pragma once
#include "../Core/Time/GameTimer.h"
#include "../Engine/ECS/ECS.h"

class GameManager
{
private:
    GameTimer timer;
    Manager manager;

    Entity playerEntity;

    float mSpawnTimer = 0.0f;
    float mSpawnInterval = 3.0f;
   
public:
    void Spawner(float deltaTime);
    void GameLoop(HINSTANCE hInstance);
};