#pragma once
#include "../Engine/ECS/ECS.h"
#include "../Render/SceneRender.h"
#include "../Engine/ECS/Systems/ParticleSystem.h"

class ScriptsTest : public Script
{
private:
	
public:
	void Init()override;
	void Update(float deltatime)override;
	void SetParticleSystem(ParticleSystem* particleSystem);

private:
	SceneRender* mScene;
	ParticleSystem* mParticleSystem = nullptr;
	Entity* mCursorEntity = nullptr;
};

