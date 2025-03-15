#pragma once
#include "ECS.h"

template <typename T>
T* Entity::GetScript()
{
    for (auto* script : scripts) {
        if (auto* castedScript = dynamic_cast<T*>(script)) {
            return castedScript;
        }
    }
    return nullptr;
}

// Component Management
template <typename T>
T& Manager::AddComponent(Entity& entity, T* component) {
    static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

    assert(component && "Component cannot be null!");

    T* ptr = component;
    ptr->mEntity = &entity;

    entity.components.push_back(ptr);

    entity.mComponentArray[GetComponentTypeID<T>()] = ptr;
    entity.mComponentBitSet[GetComponentTypeID<T>()] = true;

    ptr->Init();
    return *ptr;
}

template <typename T>
T& Manager::GetComponent(const Entity& entity) const {
    auto ptr = entity.mComponentArray[GetComponentTypeID<T>()];
    assert(ptr && "GetComponent failed: Component not found!");
    return *static_cast<T*>(ptr);
}

template <typename T>
void Manager::RemoveComponent(Entity& entity) {
    entity.mComponentArray[GetComponentTypeID<T>()] = nullptr;
    entity.mComponentBitSet[GetComponentTypeID<T>()] = false;
    entity.components.erase(std::remove_if(
        std::begin(entity.components), std::end(entity.components),
        [](Component* c) { return typeid(*c) == typeid(T); }
    ), std::end(entity.components));
}

template <typename T>
bool Manager::HasComponent(const Entity& entity) const {
    return entity.mComponentBitSet[GetComponentTypeID<T>()];
}


template <typename T>
Entity* Manager::FindEntityWithComponent() {
    for (auto* entity : GetEntities()) {
        if (HasComponent<T>(*entity)) {
            return entity;
        }
    }
    return nullptr;
}

template <typename T, typename... Args>
T& Manager::AddSystem(Args&&... args) {
    static_assert(std::is_base_of<System, T>::value, "T must be a derived class of System");

    size_t systemID = GetSystemTypeID<T>();
    assert(systemID < maxSystem && "Exceeded max number of systems!");

    if (mSystemBitSet[systemID]) {
        return *static_cast<T*>(mSystemArray[systemID]);
    }

    // Création et stockage du système
    T* system = new T(std::forward<Args>(args)...);
    mSystemArray[systemID] = system;
    mSystemBitSet[systemID] = true;
    systems.emplace_back(system); // Ajout au vector pour la gestion mémoire

    return *system;
}

template <typename T>
T& Manager::GetSystem() const {
    size_t systemID = GetSystemTypeID<T>();
    assert(systemID < maxSystem && "Invalid system ID!");
    assert(mSystemBitSet[systemID] && "System not found!");
    return *static_cast<T*>(mSystemArray[systemID]);
}

template <typename T>
bool Manager::HasScript(const Entity& entity) const {
    for (const auto* script : entity.GetScripts()) {
        if (dynamic_cast<const T*>(script) != nullptr) {
            return true;
        }
    }
    return false;
}

template <typename T>
Entity* Manager::FindEntityWithScript()
{
    auto it = scriptEntitiesIndex.find(std::type_index(typeid(T)));
    if (it != scriptEntitiesIndex.end() && !it->second.empty()) {
        return it->second.front();
    }
    return nullptr;
}

