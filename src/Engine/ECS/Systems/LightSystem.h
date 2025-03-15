#pragma once
#include "../ECS.h"
#include <DirectXMath.h>
#include "../Engine/ECS/Components/LightComponent.h"

class LightSystem : public System
{
private:
	Manager* manager;
	SceneRender* scene;

public:
	LightSystem(Manager* m);
	void Init()override;
	void Update(float dt)override;

	void SetPointLight(Entity* entity,
		DirectX::XMFLOAT3 Position,
		DirectX::XMFLOAT4 Color,
		DirectX::XMFLOAT3 Strength,
		float FalloffStart,
		float FalloffEnd);

	void SetSpotLight(Entity* entity,
		DirectX::XMFLOAT3 Position,
		DirectX::XMFLOAT4 Color,
		DirectX::XMFLOAT3 Strength,
		float FalloffStart,
		float FalloffEnd,
		float SpotPower);

	void SetDirectionalLight(Entity* entity,
		DirectX::XMFLOAT4 Color,
		DirectX::XMFLOAT3 Strength,
		float SunPhi,
		float SunTheta);
};

