#include "pch.h"
#include "HUDScript.h"

void HUDScript::Init()
{
    std::cout << "[DEBUG] HUD System Initialized" << std::endl;
}

void HUDScript::Update(float deltaTime)
{
    Render();
}

void HUDScript::Render()
{
    /*std::cout << "[HUD] Health: " << playerHealth << "%";
    std::cout << " | Shield: " << (shieldActive ? "Active" : "Inactive");
    std::cout << " | Boost: " << boostLevel << "x" << std::endl;*/
}

void HUDScript::SetPlayerHealth(float health) {
    playerHealth = health;
}

void HUDScript::SetShieldStatus(bool active) {
    shieldActive = active;
}
void HUDScript::SetBoostLevel(float level) {
    boostLevel = level;
}