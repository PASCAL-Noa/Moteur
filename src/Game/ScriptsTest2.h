#pragma once
#include "../Engine/ECS/ECS.h"

class ScriptsTest2 : public Script
{
private:
	
public:
	void Init()override;
	void Update(float deltatime)override;

private:
	SceneRender* mScene;
	float pos = 0;
};

