#pragma once

#include "../../Core/Tools/Transform.h"
#include <bitset>
#include <typeindex>
#include <unordered_map>
#include <array>

#include "../../Core/Tools/Utils.h"

class SceneRender;
class Component;
class Entity;
class Script;

constexpr std::size_t MAX_COMPONENTS = 32;
constexpr std::size_t MAX_ENTITIES = 5000;

typedef std::bitset<MAX_COMPONENTS> ComponentBitset;

using ComponentID = std::size_t;

inline ComponentID GetComponentTypeID()
{
    static ComponentID lastID = 0;
    return lastID++;
}

template <typename T> inline ComponentID GetComponentTypeID() noexcept
{
    static ComponentID typeID = GetComponentTypeID();
    return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxSystem = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
public:
    Entity* mEntity;
    virtual ~Component() = default;
    virtual void Init() {}
};

class Entity {
private:
    bool mIsAlive = true;
    SceneRender* mScene;

    ComponentArray mComponentArray = {};
    ComponentBitSet mComponentBitSet;

    std::vector<Component*> components;
    std::vector<Script*> scripts;

    int ID = 0;

public:
    Transform transform;

    explicit Entity() = default;
    Entity(SceneRender* scene);

    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;

    Entity(Entity&&) noexcept = default;
    Entity& operator=(Entity&&) noexcept = default;

    void Update(float deltaTime);
    void Destroy();
    bool IsAlive() const { return mIsAlive; }
    int GetId() const { return ID; }

    const std::vector<Script*>& GetScripts() const { return scripts; }
    
    Utils::Vector3i gridPosition;
    void UpdateGrid(int cellSize);
    //Utils::Vector3i GetGridPosition(const DirectX::XMFLOAT3& position, int cellSize)

    template <typename T>
    T* GetScript();
    
    
    SceneRender* GetScene();

    friend class Manager;
    
};

inline ComponentID GetSystemTypeID()
{
    static ComponentID lastID = 0;
    return lastID++;
}

template <typename T> 
inline ComponentID GetSystemTypeID() noexcept
{
    static ComponentID typeID = GetSystemTypeID();
    return typeID;
}

class System
{
public:
    System() = default;
    virtual ~System() = default;

    virtual void Update(float deltaTime) = 0;
    virtual void Init() = 0;

};

class Script
{
public:
    virtual ~Script() = default;

    virtual void Init() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void OnCollide(Entity* other) {}

    Entity* mEntity = nullptr;
    Manager* mScriptManager = nullptr;
};

class Manager
{
private:
    SceneRender* mScene;
    std::vector<Entity*> entities;
    std::vector<System*> systems;
    std::vector<Script*> scripts;

    std::unordered_map<std::type_index, std::vector<Entity*>> scriptEntitiesIndex;

    std::bitset<maxSystem> mSystemBitSet;
    std::array<System*, maxSystem> mSystemArray = {};

public:
    Manager() = default;

    Manager(const Manager&) = delete;
    Manager& operator=(const Manager&) = delete;

    Manager(Manager&&) = default;
    Manager& operator=(Manager&&) = default;
    void Init();
    void Update(float deltaTime);
    void Refresh();
    static bool IsEntityDead(const Entity* entity);
    void SetScene(SceneRender* scene);

    // Entity management
    Entity& CreateEntity();
    const std::vector<Entity*>& GetEntities();

    // Component management
    template <typename T>
    T& AddComponent(Entity& entity, T* component);
    template <typename T>
    T& GetComponent(const Entity& entity) const;
    template <typename T>
    void RemoveComponent(Entity& entity);
    template <typename T>
    bool HasComponent(const Entity& entity) const;
    template <typename T>
    Entity* FindEntityWithComponent();

    template<typename T, typename ...Args>
    T& AddSystem(Args && ...args);



    // System management
    template <typename T>
    T& GetSystem() const;

    //Scripts
    void AttachScript(Entity* entity, Script* script);

    template <typename T>
    bool HasScript(const Entity& entity) const;

    template <typename T>
    Entity* FindEntityWithScript();

    const std::vector<Script*>& GetScripts() const { return scripts; }
};

inline void Manager::SetScene(SceneRender* scene)
{
    mScene = scene;
}

#include "ECS.inl"
