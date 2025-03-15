#ifndef PCH_H
#define PCH_H

// Standard Libraries
#include <windows.h>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include <chrono>

// DirectX
#include <DirectXMath.h>

#pragma comment(lib, "winmm.lib")

#include "../Engine/ECS/Components/MeshRenderer.h"
#include "../Engine/ECS/Components/InputComponent.h"
#include "../Engine/ECS/Components/CameraComponent.h"
#include "../Engine/ECS/Components/AABBCollider.h"
#include "../Engine/ECS/Components/Collider.h"
#include "../Engine/ECS/Components/LightComponent.h"
#include "../Engine/ECS/Components/Rigidbody.h"


#include "../Engine/ECS/Systems/CameraSystem.h"
#include "../Engine/ECS/Systems/CollisionSystem.h"
#include "../Engine/ECS/Systems/InputSystem.h"
#include "../Engine/ECS/Systems/PhysicsSystem.h"
#include "../Engine/ECS/Systems/RenderSystem.h"
#include "../Engine/ECS/Systems/LightSystem.h"

#endif