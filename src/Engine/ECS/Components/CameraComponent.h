#pragma once

#include "../../../Core/Maths/MathHelper.h"
#include "../ECS.h"

using namespace DirectX;

class CameraComponent : public Component {
public:
	float mRadius = 5.0f;
	XMFLOAT4X4 mProj = MathHelper::Identity4x4();
	XMFLOAT4X4 mView = MathHelper::Identity4x4();
	float x = 0.0f, y = 0.0f, z = 0.0f;
	float theta = 0.0f, phi = 0.0f;
	Transform mCameraTransform;

    CameraComponent() = default;

	DirectX::XMFLOAT3 GetPosition() const {
		return { mCameraTransform.GetPosition().x,
				 mCameraTransform.GetPosition().y,
				 mCameraTransform.GetPosition().z };
	}
};
