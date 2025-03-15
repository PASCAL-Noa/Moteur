#include "pch.h"
#include "Test.h"
#include "../Core/Tools/Utils.h"

void Test::RunTests()
{
    Transform transform;

    std::cout << "### TEST COMPLET Transform ###\n";

    TestIdentity(transform);
    TestTranslation(transform);
    TestRotation(transform);
    TestRotationQuaternion(transform);
    TestScaling(transform);
    TestPosition(transform);
    TestLookAt(transform, {0.0f, 0.0f, 0.0f});
    TestLookAt(transform, {10.0f, 0.0f, 10.0f});
    TestLookAt(transform, {-10.0f, 5.0f, -5.0f});
    TestLerpPosition(transform, {10.0f, 5.0f, 0.0f});
    TestSlerpRotation(transform, {0.0f, 1.0f, 0.0f, 1.0f});
}

void Test::TestIdentity(Transform& transform)
{
    std::cout << "\n### Test Réinitialisation (Identity) ###\n";
    transform.Identity();
    Utils::PrintMatrix(transform.matrix);
    Utils::PrintVector("Position", transform.GetPosition());
    Utils::PrintVector("Scale", transform.GetScale());
    Utils::PrintVector("Forward", transform.GetForward());
    Utils::PrintVector("Right", transform.GetRight());
    Utils::PrintVector("Up", transform.GetUp());
}

void Test::TestTranslation(Transform& transform)
{
    std::cout << "\n### Test Translation (2, 3, 5) ###\n";
    transform.Translate(2.0f, 3.0f, 5.0f);
    Utils::PrintMatrix(transform.matrix);
    Utils::PrintVector("Position", transform.GetPosition());
}

void Test::TestRotation(Transform& transform)
{
    std::cout << "\n### Test Rotation (Yaw: 30°, Pitch: 45°, Roll: 60°) ###\n";
    transform.AddRotate
    (
        DirectX::XMConvertToRadians(30.0f), 
        DirectX::XMConvertToRadians(45.0f), 
        DirectX::XMConvertToRadians(60.0f)
    );
    Utils::PrintMatrix(transform.matrix);
    Utils::PrintVector("Forward", transform.GetForward());
    Utils::PrintVector("Right", transform.GetRight());
    Utils::PrintVector("Up", transform.GetUp());
    
}

void Test::TestRotationQuaternion(Transform& transform)
{
    std::cout << "\n### Test Rotation Quaternion (90° sur Y) ###\n";

    DirectX::XMFLOAT4 rotationQuat;
    DirectX::XMStoreFloat4(&rotationQuat, DirectX::XMQuaternionRotationAxis(
        DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),
        DirectX::XMConvertToRadians(90.0f)
    ));

    transform.RotateQuaternion(rotationQuat);
    Utils::PrintMatrix(transform.matrix);
    Utils::PrintVector("Forward", transform.GetForward());
    Utils::PrintVector("Right", transform.GetRight());
    Utils::PrintVector("Up", transform.GetUp());
}

void Test::TestScaling(Transform& transform)
{
    std::cout << "\n### Test Mise à l'échelle (0.5, 2.0, 1.5) ###\n";
    transform.SetScale(0.5f, 2.0f, 1.5f);
    Utils::PrintMatrix(transform.matrix);
    Utils::PrintVector("Scale", transform.GetScale());
}

void Test::TestPosition(Transform& transform)
{
    std::cout << "\n### Test Changement de Position (10, 20, -5) ###\n";
    transform.SetPosition(10.0f, 20.0f, -5.0f);
    Utils::PrintMatrix(transform.matrix);
    Utils::PrintVector("Position", transform.GetPosition());
}

void Test::TestLookAt(Transform& transform, const DirectX::XMFLOAT3& target)
{
    std::cout << "\n### Test LookAt (vers " << target.x << ", " << target.y << ", " << target.z << ") ###\n";

    // Sauvegarde de la position avant LookAt()
    DirectX::XMFLOAT3 prevPosition = transform.GetPosition();

    transform.LookAt(target);
    Utils::PrintMatrix(transform.matrix);
    Utils::PrintVector("Forward", transform.GetForward());
    Utils::PrintVector("Right", transform.GetRight());
    Utils::PrintVector("Up", transform.GetUp());

    // Vérifier que LookAt() ne change PAS la position
    DirectX::XMFLOAT3 newPosition = transform.GetPosition();
    if (prevPosition.x != newPosition.x || prevPosition.y != newPosition.y || prevPosition.z != newPosition.z)
    {
        std::cout << "[ERREUR] LookAt() a modifié la position !\n";
    }
    else
    {
        std::cout << "[CHECK] LookAt() n'a PAS modifié la position.\n";
    }
}

void Test::TestLerpPosition(Transform& transform, const DirectX::XMFLOAT3& target)
{
    std::cout << "\n### Test LerpPosition() ###\n";
    transform.LerpPosition(target, 0.5f); // Interpolation à mi-chemin
    Utils::PrintMatrix(transform.matrix);
    Utils::PrintVector("Position", transform.GetPosition());
}

void Test::TestSlerpRotation(Transform& transform, const DirectX::XMFLOAT4& targetQuat)
{
    std::cout << "\n### Test SlerpRotation() ###\n";
    transform.SlerpRotation(targetQuat, 0.5f); // Interpolation à mi-chemin
    Utils::PrintMatrix(transform.matrix);
}

