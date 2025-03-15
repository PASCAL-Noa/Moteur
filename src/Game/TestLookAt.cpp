#include "pch.h"
#include "TestLookAt.h"
#include "../Core/Tools/Utils.h"
#include "../Core/Tools/Transform.h"

void TestLookAt::RunTests()
{
    std::cout << "\n### TESTS LookAt() ###\n";
    TestLookAtOrigin();
    TestLookAtDifferentPositions();
    TestLookAtEdgeCases();
}

void TestLookAt::TestLookAtOrigin()
{
    std::cout << "\n### LookAt(0,0,0) ###\n";
    Transform transform;
    transform.SetPosition(10.0f, 20.0f, -5.0f);
    transform.LookAt({0.0f, 0.0f, 0.0f});
    
    Utils::PrintMatrix(transform.matrix);
    Utils::PrintVector("Forward", transform.GetForward());
    Utils::PrintVector("Right", transform.GetRight());
    Utils::PrintVector("Up", transform.GetUp());
}

void TestLookAt::TestLookAtDifferentPositions()
{
    std::cout << "\n### LookAt Différentes Positions ###\n";
    Transform transform;

    struct TestCase
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT3 target;
    };

    TestCase testCases[] =
    {
        {.position= {10.0f, 0.0f, 10.0f}, .target= {0.0f, 0.0f, 0.0f}},
        {.position= {0.0f, 5.0f, 5.0f}, .target= {0.0f, 0.0f, 0.0f}},
        {.position= {-10.0f, -10.0f, -10.0f}, .target= {0.0f, 0.0f, 0.0f}}
    };

    for (const auto& testCase : testCases)
    {
        std::cout << "\nTest LookAt de (" << testCase.position.x << ", "
                  << testCase.position.y << ", " << testCase.position.z << ") vers ("
                  << testCase.target.x << ", " << testCase.target.y << ", " << testCase.target.z << ")\n";
        
        transform.SetPosition(testCase.position.x, testCase.position.y, testCase.position.z);
        transform.LookAt(testCase.target);
        
        Utils::PrintMatrix(transform.matrix);
        Utils::PrintVector("Forward", transform.GetForward());
        Utils::PrintVector("Right", transform.GetRight());
        Utils::PrintVector("Up", transform.GetUp());
    }
}

void TestLookAt::TestLookAtEdgeCases()
{
    std::cout << "\n### Cas Limites LookAt ###\n";
    Transform transform;

    // Cas 1: LookAt() vers un point très proche
    transform.SetPosition(1.0f, 1.0f, 1.0f);
    transform.LookAt({1.0001f, 1.0001f, 1.0001f});
    std::cout << "\nLookAt vers un point extrêmement proche\n";
    Utils::PrintMatrix(transform.matrix);
    Utils::PrintVector("Forward", transform.GetForward());

    // Cas 2: LookAt() vers le même point (ignorer la transformation)
    transform.SetPosition(5.0f, 5.0f, 5.0f);
    transform.LookAt({5.0f, 5.0f, 5.0f});
    std::cout << "\nLookAt vers soi-même (Doit rester inchangé)\n";
    Utils::PrintMatrix(transform.matrix);
    Utils::PrintVector("Forward", transform.GetForward());
}
