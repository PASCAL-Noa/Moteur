#include "pch.h"
#include "../Components/InputComponent.h"
#include "InputSystem.h"

InputSystem::~InputSystem()
{
}

InputSystem::InputSystem(Manager* m)
    : manager(m) {
}

void InputSystem::Update(float dt)
{
    for (auto entity : manager->GetEntities()) 
    {
        if (manager->HasComponent<InputComponent>(*entity) && entity->IsAlive())
        {
            InputComponent& input = manager->GetComponent<InputComponent>(*entity);
            UpdateKeyStates();
            UpdateMousePosition();
        }
    }
}

void InputSystem::Init()
{
	ShowCursor(true);
}

void InputSystem::UpdateKeyStates() {
    for (auto entity : manager->GetEntities()) {
        if (manager->HasComponent<InputComponent>(*entity) && entity->IsAlive()) {
            auto& inputComponent = manager->GetComponent<InputComponent>(*entity);
            for (auto& [key, state] : inputComponent.mKeyStates) {
                bool isPressed = (GetAsyncKeyState(key) & 0x8000) != 0;
                if (isPressed) {
                    if (state == NONE || state == UP) {
                        state = PUSH;
                    }
                    else {
                        state = DOWN;
                    }
                }
                else {
                    if (state == DOWN || state == PUSH) {
                        state = UP;
                    }
                    else {
                        state = NONE;
                    }
                }
            }
        }
    }
}

void InputSystem::UpdateMousePosition() {
    POINT p;
    for (auto entity : manager->GetEntities()) 
    {
        if (manager->HasComponent<InputComponent>(*entity) && entity->IsAlive())
        {
            auto& inputComponent = manager->GetComponent<InputComponent>(*entity);

            if (GetCursorPos(&p)) {
                inputComponent.mMouseX = p.x;
                inputComponent.mMouseY = p.y;
            }
        }
    }
}


KeyState InputSystem::GetMouseButtonState(int button)
{
    for (auto entity : manager->GetEntities()) 
    {
        if (manager->HasComponent<InputComponent>(*entity) && entity->IsAlive())
        {
            auto& inputComponent = manager->GetComponent<InputComponent>(*entity);
            bool isPressed = (GetAsyncKeyState(button) & 0x8000) != 0;
            auto it = inputComponent.mKeyStates.find(button);
            if (it != inputComponent.mKeyStates.end()) {
                if (isPressed) {
                    if (it->second == NONE || it->second == UP) {
                        it->second = PUSH;
                    }
                    else {
                        it->second = DOWN;
                    }
                }
                else {
                    if (it->second == DOWN || it->second == PUSH) {
                        it->second = UP;
                    }
                    else {
                        it->second = NONE;
                    }
                }
                return it->second;
            }
            inputComponent.mKeyStates[button] = isPressed ? PUSH : NONE;
            return inputComponent.mKeyStates[button];
        }
    }
}

KeyState InputSystem::GetKeyState(int keyCode) const {
    for (auto entity : manager->GetEntities()) 
    {
        if (manager->HasComponent<InputComponent>(*entity) && entity->IsAlive())
        {
            auto& inputComponent = manager->GetComponent<InputComponent>(*entity);
            auto it = inputComponent.mKeyStates.find(keyCode);
            if (it != inputComponent.mKeyStates.end()) {
                return it->second;
            }
            inputComponent.mKeyStates[keyCode] = NONE;
            return NONE;
        }
    }
}

void InputSystem::ToggleCursorLock() {
	ShowCursor(!mLockCursor);
}

DirectX::XMFLOAT2 InputSystem::GetMousePosition() const {
    POINT p;
    if (GetCursorPos(&p)) {
        return { static_cast<float>(p.x), static_cast<float>(p.y) };
    }
    return { 0.0f, 0.0f };
}
