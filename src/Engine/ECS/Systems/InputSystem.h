#pragma once

#include "../ECS.h"

struct InputComponent;

class InputSystem : public System {

private:
    Manager* manager;
    bool mLockCursor = true;
public:
    InputSystem(Manager* m);
    ~InputSystem()override;
    void Update(float dt) override;
    void Init()override;

    void UpdateKeyStates();
    void UpdateMousePosition();
    KeyState GetMouseButtonState(int button);
    KeyState GetKeyState(int keyCode) const;
	void ToggleCursorLock();
    DirectX::XMFLOAT2 GetMousePosition() const;
    
};
