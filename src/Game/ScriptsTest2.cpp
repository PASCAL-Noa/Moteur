#include "pch.h"
#include "ScriptsTest2.h"

#include "../Render/MeshProperties.h"

void ScriptsTest2::Init()
{
	mScriptManager->AddComponent(*mEntity, new MeshRenderer());
	mScriptManager->AddComponent(*mEntity, new InputComponent());

	mScriptManager->GetComponent<MeshRenderer>(*mEntity).textureIndex = 10;
	mScriptManager->GetSystem<RenderSystem>().AddMesh(mEntity, "skySphere", MeshProperties::CreateCustomMesh(100.f, "../../res/Render/Mesh/ball.obj"));
	
	mEntity->transform.SetPosition(0.0f, 0.0f, 0.0f);
}

void ScriptsTest2::Update(float deltaTime)
{
    //if (!mEntity) return;

    //Entity* cameraEntity = mEntity->GetScene()->GetCameraEntity();
    //if (!cameraEntity) return;

    //DirectX::XMFLOAT3 cameraPosition = cameraEntity->transform.GetPosition();
    //DirectX::XMFLOAT3 forward = cameraEntity->transform.GetForward();

    //DirectX::XMFLOAT3 playerPosition = {
    //    cameraPosition.x + forward.x ,
    //    cameraPosition.y,
    //    cameraPosition.z + forward.z
    //};

    //mEntity->transform.SetPosition(playerPosition.x, playerPosition.y, playerPosition.z);

	InputSystem& inputSystem = mScriptManager->GetSystem<InputSystem>();
	InputComponent& inputComponent = mScriptManager->GetComponent<InputComponent>(*mEntity);

	if (inputSystem.GetKeyState('K') == KeyState::DOWN) {
		Entity* projectile = &mScriptManager->CreateEntity();
		mScriptManager->AddComponent(*projectile, new MeshRenderer);
		mScriptManager->GetComponent<MeshRenderer>(*projectile).textureIndex = 1;
		mScriptManager->GetSystem<RenderSystem>().AddMesh(projectile, "proj", MeshProperties::CreateBox(0.5f));
		
		projectile->transform.SetPosition(projectile->transform.GetPosition().x + pos, projectile->transform.GetPosition().y, projectile->transform.GetPosition().z);
		pos += 0.01;
		if (projectile->transform.GetPosition().x >= 1) {
			projectile->Destroy();
		}
	}
}




