#include "pch.h"
#include "ScriptsTest.h"
#include "../Render/MeshProperties.h"
#include "../Engine/ECS/Components/Rigidbody.h"

void ScriptsTest::Init()
{
	mScriptManager->AddComponent(*mEntity, new MeshRenderer());
	mScriptManager->AddComponent(*mEntity, new LightComponent());
	mScriptManager->AddComponent(*mEntity, new InputComponent());

	mScriptManager->GetComponent<MeshRenderer>(*mEntity).textureIndex = 7;
	mScriptManager->GetSystem<LightSystem>().SetDirectionalLight(mEntity, XMFLOAT4(1, 1, 1, 1), XMFLOAT3(0.8f, 0.8f, 0.8f), 1.5*XM_PI, 0.5* XM_PI);
	mScriptManager->GetSystem<RenderSystem>().AddMesh(mEntity, "bbb", MeshProperties::CreateCustomMesh(0.8f, "../../res/Render/Mesh/ship.obj"));
	
	mEntity->transform.SetPosition(0.0f, 10.0f, 0.0f);

    // Create Cursor Entity
    mCursorEntity = &mScriptManager->CreateEntity();
    mScriptManager->AddComponent(*mCursorEntity, new MeshRenderer());
    mScriptManager->GetComponent<MeshRenderer>(*mCursorEntity).textureIndex = 5; 
    mScriptManager->GetSystem<RenderSystem>().AddMesh(mCursorEntity, "cursor", MeshProperties::CreateBox(0.3f));
}

void ScriptsTest::Update(float deltaTime)
{
    

    Entity* cameraEntity = mEntity->GetScene()->GetCameraEntity();
	Transform cameraTrans = mScriptManager->GetComponent<CameraComponent>(*cameraEntity).mCameraTransform;
    if (!cameraEntity) return;

	static bool insideMesh = false;
	static auto lastSwitchTime = std::chrono::high_resolution_clock::now();

	auto now = std::chrono::high_resolution_clock::now();
	float elapsedTime = std::chrono::duration<float>(now - lastSwitchTime).count();

    DirectX::XMFLOAT3 cameraPosition = cameraEntity->transform.GetPosition();
    DirectX::XMFLOAT3 forward = cameraEntity->transform.GetForward();

    float distanceInFront = insideMesh ? -1.0f : 13.0f;
    float heightOffset = insideMesh ? -1.5f : -5.0f;

	DirectX::XMFLOAT3 playerPosition = {
		cameraPosition.x + forward.x * distanceInFront,
		cameraPosition.y + heightOffset,
		cameraPosition.z + forward.z * distanceInFront
	};

	mEntity->transform.SetRotation(cameraTrans.GetRotation().y, cameraTrans.GetRotation().x, cameraTrans.GetRotation().z);

    if (elapsedTime >= 0.2f) {
        mParticleSystem->CreateParticles(DirectX::XMFLOAT3(playerPosition.x - 1.0f, playerPosition.y - 0.4f, playerPosition.z - 2.0f), 0.4f, 8.f, 7, 3, ParticleType::TRAIL);
        mParticleSystem->CreateParticles(DirectX::XMFLOAT3(playerPosition.x + 1.0f, playerPosition.y - 0.4f, playerPosition.z - 2.0f), 0.4f, 8.f, 7, 3, ParticleType::TRAIL);

        if ((mScriptManager->GetSystem<InputSystem>().GetKeyState('M') == DOWN)) {
            mParticleSystem->CreateParticles(DirectX::XMFLOAT3(playerPosition.x, playerPosition.y + 3.0f, playerPosition.z), 1.0f, 8.f, 6, 20, ParticleType::EXPLOSION);
            Entity* projectile = &mScriptManager->CreateEntity();
            projectile->transform.SetPosition(mEntity->transform.GetPosition().x, mEntity->transform.GetPosition().y + 3, mEntity->transform.GetPosition().z);
            mScriptManager->AddComponent(*projectile, new MeshRenderer);
            mScriptManager->AddComponent(*projectile, new Rigidbody);
            mScriptManager->AddComponent(*projectile, new Collider);
            mScriptManager->GetComponent<Rigidbody>(*projectile).velocity.z = 3000.0f * deltaTime;
            mScriptManager->GetComponent<MeshRenderer>(*projectile).textureIndex = 1;
            mScriptManager->GetSystem<RenderSystem>().AddMesh(projectile, "proj", MeshProperties::CreateSphere(0.5f, 6, 6));
        }

        if ((GetAsyncKeyState('O') & 0x8000)) {
            insideMesh = !insideMesh;
        }

        lastSwitchTime = now;
    }

    mParticleSystem->Update(deltaTime);
    mEntity->transform.SetPosition(playerPosition.x, playerPosition.y, playerPosition.z);

    mCursorEntity->transform.SetPosition(cameraPosition.x, cameraPosition.y, playerPosition.z + 5.0f);
}

void ScriptsTest::SetParticleSystem(ParticleSystem* particleSystem) {
    mParticleSystem = particleSystem;
}


