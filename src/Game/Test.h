#pragma once
#include "../Core/Tools/Transform.h"

class Test
{
public:
    static void RunTests();

    static void TestIdentity(Transform& transform);
    static void TestTranslation(Transform& transform);
    static void TestRotation(Transform& transform);
    static void TestRotationQuaternion(Transform& transform);
    static void TestScaling(Transform& transform);
    static void TestPosition(Transform& transform);
    static void TestLookAt(Transform& transform, const DirectX::XMFLOAT3& target);
    static void TestLerpPosition(Transform& transform, const DirectX::XMFLOAT3& target);
    static void TestSlerpRotation(Transform& transform, const DirectX::XMFLOAT4& targetQuat);
};
