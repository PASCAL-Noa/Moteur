#include "pch.h"
#include "LightSystem.h"
#include "../Render/SceneRender.h"

LightSystem::LightSystem(Manager* m)
{
	manager = m;
}

void LightSystem::Init()
{
	for (auto& entity : manager->GetEntities()) {
		if (manager->HasComponent<LightComponent>(*entity) && entity->IsAlive()) {
			auto& lightComponent = manager->GetComponent<LightComponent>(*entity);
			lightComponent.lightIndex += 1;
		}
	}
}

void LightSystem::Update(float dt)
{

	for (auto& entity : manager->GetEntities()) {
		if (entity->IsAlive() && manager->HasComponent<LightComponent>(*entity)) {
			auto& lightComponent = manager->GetComponent<LightComponent>(*entity);
			scene = entity->GetScene();
			sLight& light = scene->GetMainPass().light[lightComponent.lightIndex];

			switch (lightComponent.mLightType)
			{
			case LightComponent::LightType::SpotLight:
				light.Position = lightComponent.Position;
				light.Strength = lightComponent.Strength;
				light.FalloffStart = lightComponent.FalloffStart;
				light.FalloffEnd = lightComponent.FalloffEnd;
				light.Color = lightComponent.Color;
				break;

			case LightComponent::LightType::DirectionalLight:
				light.Direction = lightComponent.Direction;
				light.Strength = lightComponent.Strength;
				light.Color = lightComponent.Color;

				break;

			case LightComponent::LightType::PointLight:
				light.Position = lightComponent.Position;
				light.Strength = lightComponent.Strength;
				light.FalloffStart = lightComponent.FalloffStart;
				light.FalloffEnd = lightComponent.FalloffEnd;
				light.Color = lightComponent.Color;

				break;

			default:
				break;
			}
		}
	}
	
	scene;
	scene->GetPass()->CopyData(0, scene->GetMainPass());
}



void LightSystem::SetSpotLight(Entity* entity, 
	DirectX::XMFLOAT3 Position, 
	DirectX::XMFLOAT4 Color, 
	DirectX::XMFLOAT3 Strength, 
	float FalloffStart, 
	float FalloffEnd, 
	float SpotPower)
{
	SceneRender* scene = entity->GetScene();
	auto& lightComponent = manager->GetComponent<LightComponent>(*entity);
	lightComponent.mLightType = LightComponent::LightType::SpotLight;
	scene->GetMainPass().light[lightComponent.lightIndex].Position = Position;
	scene->GetMainPass().light[lightComponent.lightIndex].Strength = Strength;
	scene->GetMainPass().light[lightComponent.lightIndex].FalloffStart = FalloffStart;
	scene->GetMainPass().light[lightComponent.lightIndex].FalloffEnd = FalloffEnd;
	scene->GetMainPass().light[lightComponent.lightIndex].Color = Color;
}

void LightSystem::SetDirectionalLight(Entity* entity, 
	DirectX::XMFLOAT4 Color, 
	DirectX::XMFLOAT3 Strength,
	float SunPhi,
	float SunTheta)
{
	SceneRender* scene = entity->GetScene();
	auto& lightComponent = manager->GetComponent<LightComponent>(*entity);
	sLight& light = scene->GetMainPass().light[lightComponent.lightIndex];

	lightComponent.mLightType = LightComponent::LightType::DirectionalLight;

	lightComponent.Color = Color;
	lightComponent.Strength = Strength;
	lightComponent.mSunPhi = SunPhi;
	lightComponent.mSunTheta = SunTheta;

	XMVECTOR lightDir = -MathHelper::SphericalToCartesian(1.0f, SunTheta, SunPhi);
	XMStoreFloat3(&light.Direction, lightDir);
	XMStoreFloat3(&lightComponent.Direction, lightDir);


}



void LightSystem::SetPointLight(Entity* entity,
	DirectX::XMFLOAT3 Position,
	DirectX::XMFLOAT4 Color,
	DirectX::XMFLOAT3 Strength,
	float FalloffStart,
	float FalloffEnd)
{
	if (entity->IsAlive() && manager->HasComponent<LightComponent>(*entity))
	{
		SceneRender* scene = entity->GetScene();
		auto& lightComponent = manager->GetComponent<LightComponent>(*entity);
		lightComponent.mLightType = LightComponent::LightType::PointLight;

		auto& light = scene->GetMainPass().light[lightComponent.lightIndex];
		light.Position = Position;
		light.Strength = Strength;
		light.FalloffStart = FalloffStart;
		light.FalloffEnd = FalloffEnd;
		light.Color = Color;

		lightComponent.Position = Position;
		lightComponent.Color = Color;
		lightComponent.Strength = Strength;
		lightComponent.FalloffStart = FalloffStart;
		lightComponent.FalloffEnd = FalloffEnd;
	}
}
