#pragma once

class DirectX12Renderer
{
public:
    DirectX12Renderer();
    ~DirectX12Renderer();

    bool Initialize(HWND hwnd);
    void FlushCommandQueue();
    void ResizeBuffers(UINT width, UINT height);
private:
    static void EnableDebugLayer();
    bool CreateDXGIFactory();
    bool CreateDevice();
    void CreateFence();
    void RetrieveDescriptorSizes();
    void CheckMSAA();
    void CreateCommandObjects();
    void CreateSwapChain(HWND hwnd);
    void CreateDescriptorHeaps();
    void CreateRenderTargetViews();
    void CreateDepthStencilBuffer();
    void SetViewportAndScissorRect();
    void Cleanup();
    
    ID3D12Device* md3dDevice = nullptr;
    IDXGIFactory4* mdxgiFactory = nullptr;
    ID3D12CommandQueue* mCommandQueue = nullptr;
    ID3D12CommandAllocator* mCommandAllocator = nullptr;
    ID3D12GraphicsCommandList* mCommandList = nullptr;
    IDXGISwapChain* mSwapChain = nullptr;
    ID3D12DescriptorHeap* mRtvHeap = nullptr;
    ID3D12DescriptorHeap* mDsvHeap = nullptr;
    ID3D12Fence* mFence = nullptr;
    ID3D12Resource* mSwapChainBuffer[2] = { nullptr, nullptr };
    ID3D12Resource* mDepthStencilBuffer = nullptr;

    UINT mRtvDescriptorSize = 0;
    UINT mDsvDescriptorSize = 0;
    UINT mCbvSrvDescriptorSize = 0;

    bool m4xMsaaState = false;
    UINT m4xMsaaQuality = 0;
    
    DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;    
    static const int SwapChainBufferCount = 2;
    UINT64 mCurrentFence = 0;
    
    D3D12_VIEWPORT mScreenViewport;
    D3D12_RECT mScissorRect;
};
