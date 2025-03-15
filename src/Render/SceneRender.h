#pragma once
#include "d3dApp.h"
#include "UploadBuffer.h"
#include "../Core//Maths/MathHelper.h"
#include "../Engine/ECS/Components/CameraComponent.h"
#include "../Engine/ECS/Components/MeshRenderer.h"

using namespace DirectX;

class SceneRender : public d3dApp
{
private:
    ID3DBlob* mvsByteCode = nullptr;
    ID3DBlob* mpsByteCode = nullptr;
    std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;
    ID3D12RootSignature* mRootSignature = nullptr;
    ID3D12PipelineState* mPSO = nullptr;
    ID3D12DescriptorHeap* mCbvHeap = nullptr;
    ID3D12DescriptorHeap* mSrvHeap = nullptr;
    XMFLOAT4X4 mWorld = MathHelper::Identity4x4();
    XMFLOAT4X4 mView = MathHelper::Identity4x4();
    XMFLOAT3 mLightDirection = XMFLOAT3(0.0f, 10.0f, 0.0f);

    PassConstants mMainPass;
    UploadBuffer<PassConstants>* mPassCB = nullptr;

    UINT mCbvHeapOffset = 0;

    
    
    float Rz = -10.0f;
    float Rx = 0.0f;
    float Ry = 0.0f;
    uint32_t entityCount = 0;

    std::vector < const wchar_t* > textureFilesCopy;
    bool isLoaded = false;

    float mSunTheta = 1.25f * XM_PI;
    float mSunPhi = XM_PIDIV4;

public:
    SceneRender(HINSTANCE hInstance);
    ~SceneRender();
    bool Init();
    void Draw()override;
    void Update()override;

    void UpdateObject();
    void UpdatePass();
    void BuildShadersAndInputLayout();
    void buildPSO();
    void BuildDescriptorHeaps(std::size_t size);
    void BuildConstantBuffers();
    void BuildRootSignature();
    void PrintMatrix(const XMMATRIX& matrix);
    void BuildTexture(std::vector < const wchar_t* >);

    
    ID3D12DescriptorHeap* GetCbvHeap() { return mCbvHeap; }
    UINT GetCbvHeapOffset() { return mCbvHeapOffset; }
    UINT GetCbvSrvDescriptorSize() { return mCbvSrvDescriptorSize; }
    ID3D12Device* GetD3DDevice() { return md3dDevice; }
    ID3D12GraphicsCommandList* GetCommandList() { return mCommandList; }
    ID3D12CommandAllocator* GetDirectCmdListAlloc() { return mDirectCmdListAlloc; }
    ID3D12CommandQueue* GetCommandQueue() { return mCommandQueue; }
    PassConstants& GetMainPass() { return mMainPass; }
    SceneRender* GetSceneRender() { return this; }
    UploadBuffer<PassConstants>* GetPass() { return mPassCB; }
    
};
