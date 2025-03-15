#include "pch.h"
#include "GameManager.h"
#include "ECS/ECS.h"
#include "../Render/SceneRender.h"
#include "ScriptsTest.h"
#include "ScriptsTest2.h"
#include "../Engine/ECS/Systems/ParticleSystem.h"
#include "Game/SpawnScript.h"
#include "EnemyScript.h"

void GameManager::Spawner(float deltaTime)
{
    mSpawnTimer += deltaTime;

    if (mSpawnTimer >= mSpawnInterval) {
        mSpawnTimer = 0.0f;

        int randomChoice = rand() % 2;
        if (randomChoice == 0)
            SpawnScript::SpawnMeteor(manager);
        else
            SpawnScript::SpawnHomingMeteor(manager);

        SpawnScript::SpawnTurret(manager);
    }
}

void GameManager::GameLoop(HINSTANCE hInstance) {
    try
    {
        SceneRender app(hInstance);

        std::vector < const wchar_t * > textureFiles = {
            L"../../res/Render/Texture/texture.dds", // 0 
            L"../../res/Render/Texture/texture2.dds", // 1
            L"../../res/Render/Texture/mario.dds", // 2
            L"../../res/Render/Texture/Grass_Block_TEX.dds", // 3
            L"../../res/Render/Texture/space2.dds", // 4
            L"../../res/Render/Texture/cursor.dds", // 5
            L"../../res/Render/Texture/red.dds", // 6
            L"../../res/Render/Texture/gray.dds", // 7
            L"../../res/Render/Texture/bg.dds", // 8
            L"../../res/Render/Texture/nebulaBackground.dds", // 9
            L"../../res/Render/Texture/giole.dds", // 10
        };

		app.BuildTexture(textureFiles);

        if (!app.Init())
        {
            std::cerr << "Initialization failed." << std::endl;
            return;
        }

        ScriptsTest script;
        ScriptsTest2 skyScript;
        EnemyScript enemyScript;
        Manager& manager = app.GetManager();

        Entity* ff = &manager.CreateEntity();
        Entity* dede = &manager.CreateEntity();
        Entity* SkySphere = &manager.CreateEntity();
        Entity* Enemy = &manager.CreateEntity();

        ParticleSystem particleSystem(&manager);
        script.SetParticleSystem(&particleSystem);
        manager.AttachScript(ff, &script);
        manager.AttachScript(SkySphere, &skyScript);
        manager.AttachScript(Enemy, &enemyScript);


        app.Run();

    
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}
