#include "pch.h"
#include "EnemyScript.h"
#include "../Render/MeshProperties.h"

void EnemyScript::Init()
{
    cameraEntity = mScriptManager->FindEntityWithComponent<CameraComponent>();
}

float getRandomInRange(float min, float max)
{
    return min + (rand() / (float)RAND_MAX) * (max - min);
}

float getRandomDirection()
{
    return rand() % 2 == 0 ? -1.0f : 1.0f; 
}

void EnemyScript::Update(float deltaTime)
{
    SpawnProgress -= deltaTime;
    if (SpawnProgress < 0) {
        Entity* enemy = &mScriptManager->CreateEntity();
        enemy->transform.SetPosition(
            cameraEntity->transform.GetPosition().x + getRandomInRange(-50, 50),
            cameraEntity->transform.GetPosition().y - 3,
            cameraEntity->transform.GetPosition().z + getRandomInRange(120, 150)
        );
        enemy->transform.SetRotation(3.2f, 0.0f, 0.0f);
        mScriptManager->AddComponent(*enemy, new MeshRenderer);
        mScriptManager->AddComponent(*enemy, new Rigidbody);
        mScriptManager->AddComponent(*enemy, new Collider);
        mScriptManager->GetComponent<Rigidbody>(*enemy).velocity.z = 500.0f * deltaTime * -1;
        mScriptManager->GetComponent<MeshRenderer>(*enemy).textureIndex = 1;
        mScriptManager->GetSystem<RenderSystem>().AddMesh(enemy, "enemy", MeshProperties::CreateCustomMesh(0.05f, "../../res/Render/Mesh/Torpedo.obj"));
        SpawnProgress = SpawnTime;
    }
}




