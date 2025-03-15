#include "pch.h"
#include "ECS.h"

#include "Components/MeshRenderer.h"

Entity::Entity(SceneRender* scene) : mScene(scene) {}

void Entity::Update(float deltaTime)
{
}

void Entity::Destroy()
{
    mIsAlive = false;
}

SceneRender* Entity::GetScene()
{
    return mScene;
}

void Entity::UpdateGrid(int cellSize)
{
    gridPosition.x = static_cast<int>(transform.GetPosition().x) / cellSize;
    gridPosition.y = static_cast<int>(transform.GetPosition().y) / cellSize;
    gridPosition.z =  static_cast<int>(transform.GetPosition().z) / cellSize;
    
}

// =================================================================================================
// Manager Section
// =================================================================================================


void Manager::Init()
{
    scripts.erase(std::remove(scripts.begin(), scripts.end(), nullptr), scripts.end());

    for (auto& s : scripts) {
        s->Init();
    }
    for (auto& system : systems) {
        system->Init();
    }

}

void Manager::Update(float deltaTime)
{
    
    for (auto& system : systems) {
        system->Update(deltaTime);
    }
    for (auto& e : entities) {
        e->Update(deltaTime);
    }
    for (auto& s : scripts) {
        s->Update(deltaTime);
    }
    Refresh();
}

void Manager::Refresh()
{
    // On supprime d'abord la mémoire des entités mortes
    for (auto it = entities.begin(); it != entities.end(); ) {
        Entity* entity = *it;
        if (!entity->IsAlive()) {
            
            // 🔹 Supprimer tous les composants
            for (auto* component : entity->components) {
                delete component;
            }
            entity->components.clear();

            
            delete entity;
            it = entities.erase(it); 
        } 
        else {
            ++it;
        }
    }
}





bool Manager::IsEntityDead(const Entity* entity)
{
    return !entity->IsAlive();
}

// Entity Management
Entity& Manager::CreateEntity()
{
    Entity* e = new Entity(mScene);
    e->ID += 1;
    entities.push_back(e);
    return *e;
}

const std::vector<Entity*>& Manager::GetEntities()
{
    return entities;
}

// void Manager::AttachScript(Entity* entity, Script* script)
// {
//     if (!entity || !script) return;
//
//     script->mEntity = entity;
//     script->mScriptManager = this;
//     scripts.push_back(script);
//
//     entity->scripts.push_back(script);
// }

void Manager::AttachScript(Entity* entity, Script* script)
{
    if (!entity || !script) return;

    script->mEntity = entity;
    script->mScriptManager = this;
    scripts.push_back(script);

    entity->scripts.push_back(script);

    scriptEntitiesIndex[std::type_index(typeid(*script))].push_back(entity);
}
