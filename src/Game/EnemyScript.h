#pragma once
#include "../Engine/ECS/ECS.h"

class Entity;
class EnemyScript : public Script
{
private:
	
public:
	void Init()override;
	void Update(float deltatime)override;

private:
	SceneRender* mScene;
	float pos = 0;
	float SpawnProgress;
	float SpawnTime = 5.0f;
	Entity* cameraEntity;
};

