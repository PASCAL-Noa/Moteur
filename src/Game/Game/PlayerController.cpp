 #include "pch.h"
#include "PlayerController.h"
#include "../../Engine/ECS/ECS.h"
#include "../../Engine/ECS/Components/InputComponent.h"
#include "../../Engine/ECS/Components/Rigidbody.h"
#include "../../Engine/ECS/Components/Collider.h"
#include "../../Engine/ECS/Components/MeshRenderer.h"

constexpr float BOOST_SPEED = 10.0f;
constexpr float BOOST_TIME = 1.5f;
constexpr float BOOST_COOLDOWN = 3.0f;
constexpr float PROJECTILE_SPEED = 10.0f;

void PlayerController::Init() {
    inputSystem = &mScriptManager->GetSystem<InputSystem>();

    if (!mScriptManager->HasComponent<Rigidbody>(*mEntity)) {
        std::cout << "[DEBUG] PlayerController::Init() - Rigidbody ajouté au joueur" << std::endl;
        mScriptManager->AddComponent(*mEntity, new Rigidbody());
    }

    std::cout << "[DEBUG] PlayerController initialisé" << std::endl;
}

void PlayerController::Update(float deltaTime) {
    HandleMovement(deltaTime);
    HandleBoost(deltaTime);
    HandleShooting();
    HandleShield(deltaTime);
    EnforceBoundaries();
}

void PlayerController::HandleMovement(float deltaTime) {

    Rigidbody& rb = mScriptManager->GetComponent<Rigidbody>(*mEntity);
    Utils::Vector3f velocity(0.0f, 0.0f, 0.0f);

    float moveSpeed = isBoosting ? (speed * boostMultiplier) : speed;

    if (inputSystem->GetKeyState('Q') == KeyState::DOWN) velocity.x -= moveSpeed;
    if (inputSystem->GetKeyState('D') == KeyState::DOWN) velocity.x += moveSpeed;
    if (inputSystem->GetKeyState('Z') == KeyState::DOWN) velocity.y += moveSpeed;
    if (inputSystem->GetKeyState('S') == KeyState::DOWN) velocity.y -= moveSpeed;

    rb.velocity = velocity;
}

void PlayerController::HandleBoost(float deltaTime) {
    static float boostTimeLeft = 0.0f;
    static float boostCooldownLeft = 0.0f;

    if (boostCooldownLeft > 0) {
        boostCooldownLeft -= deltaTime;
    }

    if (boostTimeLeft > 0) {
        boostTimeLeft -= deltaTime;
        if (boostTimeLeft <= 0) {
            isBoosting = false;
            boostCooldownLeft = BOOST_COOLDOWN;
        }
    }

    if (inputSystem->GetKeyState(VK_SHIFT) == KeyState::PUSH && boostCooldownLeft <= 0) {
        isBoosting = true;
        boostTimeLeft = BOOST_TIME;
    }
}

void PlayerController::HandleShooting() {
    if (inputSystem->GetMouseButtonState(0) == KeyState::PUSH) { // Left Click
        DirectX::XMFLOAT3 targetPos = GetMouseWorldPosition();
        DirectX::XMFLOAT3 playerPos = mEntity->transform.GetPosition();

        DirectX::XMFLOAT3 direction = {
            targetPos.x - playerPos.x,
            targetPos.y - playerPos.y,
            targetPos.z - playerPos.z
        };
        direction = Utils::Normalize(direction);

        // Création du projectile
        Entity* projectile = &mScriptManager->CreateEntity();
        mScriptManager->AddComponent(*projectile, new MeshRenderer());
        mScriptManager->GetSystem<RenderSystem>().AddMesh(projectile, "Projectile", MeshProperties::CreateBox(0.2f));
        mScriptManager->AddComponent(*projectile, new Rigidbody());
        mScriptManager->GetComponent<Rigidbody>(*projectile).velocity =
            Utils::Vector3f(direction.x * PROJECTILE_SPEED, direction.y * PROJECTILE_SPEED, direction.z * PROJECTILE_SPEED);
        mScriptManager->AddComponent(*projectile, new Collider());
        mScriptManager->GetComponent<Collider>(*projectile).size = Utils::Vector3f(0.2f, 0.2f, 0.2f);
        projectile->transform.SetPosition(playerPos.x, playerPos.y, playerPos.z);

        std::cout << "[DEBUG] Projectile tiré vers (" << targetPos.x << ", " << targetPos.y << ", " << targetPos.z << ")" << std::endl;
    }
}

void PlayerController::HandleShield(float deltaTime) {
    static float shieldTimeLeft = 0.0f;

    if (shieldTimeLeft > 0) {
        shieldTimeLeft -= deltaTime;
    }

    if (inputSystem->GetKeyState('E') == KeyState::PUSH && shieldTimeLeft <= 0.0f) {
        shieldActive = true;
        shieldTimeLeft = 5.0f;  // 5 secondes de cooldown
        std::cout << "[DEBUG] Bouclier activé !" << std::endl;
    }

    if (shieldActive && shieldTimeLeft <= 2.0f) {
        shieldActive = false;
        std::cout << "[DEBUG] Bouclier désactivé." << std::endl;
    }
}

void PlayerController::EnforceBoundaries() {
    DirectX::XMFLOAT3 position = mEntity->transform.GetPosition();

    position.x = std::clamp(position.x, minX, maxX);
    position.y = std::clamp(position.y, minY, maxY);

    mEntity->transform.SetPosition(position.x, position.y, position.z);
}

DirectX::XMFLOAT3 PlayerController::GetMouseWorldPosition() {
    // Récupération de l'entité caméra
    Entity* cameraEntity = mScriptManager->FindEntityWithComponent<CameraComponent>();
    if (!cameraEntity) {
        std::cerr << "[ERREUR] Aucune caméra trouvée !" << std::endl;
        return {0.0f, 0.0f, 0.0f};
    }

    CameraComponent& camera = mScriptManager->GetComponent<CameraComponent>(*cameraEntity);
    DirectX::XMFLOAT2 mousePos = inputSystem->GetMousePosition();

    // Conversion de la position écran en rayons 3D
    DirectX::XMFLOAT3 rayOrigin, rayDirection;
    Utils::ScreenToWorldRay(mousePos, camera, rayOrigin, rayDirection);

    // Calcul du point d'impact au niveau du sol (y = 0)
    float t = -rayOrigin.y / rayDirection.y; // Résout y=0
    DirectX::XMFLOAT3 worldPosition = {
        rayOrigin.x + t * rayDirection.x,
        0.0f, // On considère que le sol est à y = 0
        rayOrigin.z + t * rayDirection.z
    };

    return worldPosition;
}
