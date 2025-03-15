#include "pch.h"
#include "DirectX12Renderer.h"

DirectX12Renderer::DirectX12Renderer() = default;

DirectX12Renderer::~DirectX12Renderer()
{
    Cleanup();
}

void DirectX12Renderer::Cleanup()
{
    for (int i = 0; i < SwapChainBufferCount; i++)
    {
        if (mSwapChainBuffer[i])
        {
            mSwapChainBuffer[i]->Release();
            mSwapChainBuffer[i] = nullptr;
        }
    }

    if (mDepthStencilBuffer)
    {
        mDepthStencilBuffer->Release();
        mDepthStencilBuffer = nullptr;
    }
    
    if (mSwapChain)
    {
        mSwapChain->Release();
        mSwapChain = nullptr;
    }
    
    if (mRtvHeap)
    {
        mRtvHeap->Release();
        mRtvHeap = nullptr;
    }

    if (mDsvHeap)
    {
        mDsvHeap->Release();
        mDsvHeap = nullptr;
    }
    
    if (mFence)
    {
        mFence->Release();
        mFence = nullptr;
    }
    
    if (md3dDevice)
    {
        md3dDevice->Release();
        md3dDevice = nullptr;
    }
}

bool DirectX12Renderer::Initialize(HWND hwnd)
{
    EnableDebugLayer();

    if (CreateDXGIFactory() == false)
        return false;

    if (CreateDevice() == false)
        return false;

    CreateFence();
    RetrieveDescriptorSizes();
    CheckMSAA();
    CreateCommandObjects();
    CreateSwapChain(hwnd);
    CreateDescriptorHeaps();
    CreateRenderTargetViews();
    CreateDepthStencilBuffer();
    SetViewportAndScissorRect();
    
    return true;
}

void DirectX12Renderer::EnableDebugLayer()
{
#if defined(DEBUG) || defined(_DEBUG)
    ID3D12Debug* debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
    {
        debugController->EnableDebugLayer();
        debugController->Release();
    }
#endif
}

bool DirectX12Renderer::CreateDXGIFactory()
{
    return SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&mdxgiFactory)));
}

bool DirectX12Renderer::CreateDevice()
{
    HRESULT hardwareResult = D3D12CreateDevice
    (
        nullptr,
        D3D_FEATURE_LEVEL_11_0,
        IID_PPV_ARGS(&md3dDevice)
    );

    if (FAILED(hardwareResult))
    {
        IDXGIAdapter* pWarpAdapter;
        if (SUCCEEDED(mdxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter))))
        {
            D3D12CreateDevice
            (
                pWarpAdapter,
                D3D_FEATURE_LEVEL_11_0,
                IID_PPV_ARGS(&md3dDevice)
            );
            pWarpAdapter->Release();
        }
        else
        {
            return false;
        }
    }
    return true;
}

void DirectX12Renderer::CreateFence()
{
    md3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));
}

void DirectX12Renderer::RetrieveDescriptorSizes()
{
    mRtvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    mDsvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    mCbvSrvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void DirectX12Renderer::CheckMSAA()
{
    D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
    msQualityLevels.Format = mBackBufferFormat;
    msQualityLevels.SampleCount = 4;
    msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
    msQualityLevels.NumQualityLevels = 0;

    md3dDevice->CheckFeatureSupport
    (
        D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
        &msQualityLevels,
        sizeof(msQualityLevels)
    );

    m4xMsaaQuality = msQualityLevels.NumQualityLevels;
}

void DirectX12Renderer::CreateCommandObjects()
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

    md3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue));
    md3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocator));
    md3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator, nullptr, IID_PPV_ARGS(&mCommandList));
    mCommandList->Close();
}

void DirectX12Renderer::CreateSwapChain(HWND hwnd)
{
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferDesc.Width = 800;
    sd.BufferDesc.Height = 600;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Format = mBackBufferFormat;
    sd.SampleDesc.Count = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = SwapChainBufferCount;
    sd.OutputWindow = hwnd;
    sd.Windowed = true;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    mdxgiFactory->CreateSwapChain(mCommandQueue, &sd, &mSwapChain);
}

void DirectX12Renderer::CreateDescriptorHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
    rtvHeapDesc.NumDescriptors = SwapChainBufferCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    rtvHeapDesc.NodeMask = 0;
    md3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&mRtvHeap));
}

void DirectX12Renderer::FlushCommandQueue()
{
    mCurrentFence++;

    mCommandQueue->Signal(mFence, mCurrentFence);

    if (mFence->GetCompletedValue() < mCurrentFence)
    {
        HANDLE eventHandle = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
        mFence->SetEventOnCompletion(mCurrentFence, eventHandle);
        WaitForSingleObject(eventHandle, INFINITE);
        CloseHandle(eventHandle);
    }
}

void DirectX12Renderer::ResizeBuffers(UINT width, UINT height)
{
    if (mSwapChain)
    {
        FlushCommandQueue();

        // Libérer les anciens buffers
        for (int i = 0; i < SwapChainBufferCount; i++)
        {
            if (mSwapChainBuffer[i])
            {
                mSwapChainBuffer[i]->Release();
                mSwapChainBuffer[i] = nullptr;
            }
        }

        if (mDepthStencilBuffer)
        {
            mDepthStencilBuffer->Release();
            mDepthStencilBuffer = nullptr;
        }

        // Redimensionner la SwapChain
        mSwapChain->ResizeBuffers(
            SwapChainBufferCount,
            width, height,
            mBackBufferFormat,
            DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

        // Recréer les buffers de rendu
        CreateRenderTargetViews();
        CreateDepthStencilBuffer();

        // Mettre à jour le viewport et le scissor rect
        SetViewportAndScissorRect();
    }
}


void DirectX12Renderer::SetViewportAndScissorRect()
{
    mScreenViewport.TopLeftX = 0;
    mScreenViewport.TopLeftY = 0;
    mScreenViewport.Width = 800.0f;
    mScreenViewport.Height = 600.0f;
    mScreenViewport.MinDepth = 0.0f;
    mScreenViewport.MaxDepth = 1.0f;

    mScissorRect = { 0, 0, 800, 600 };
}
