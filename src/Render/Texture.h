#pragma once
#include <d3d12.h>

class TextureLoader {
public:
    TextureLoader();
    void LoadTexture(ID3D12GraphicsCommandList* cmdList, ID3D12DescriptorHeap* srvHeap, const wchar_t * filename);
	void SetDevice(ID3D12Device* device);
    D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle() const { return mGpuDescriptorHandle; }
    ID3D12Resource* GetTexture() { return mTexture; }
	ID3D12Resource* GetTextureUploadHeap() { return mTextureUploadHeap; }

    D3D12_GPU_DESCRIPTOR_HANDLE mGpuDescriptorHandle;

protected:
    ID3D12Resource* mTexture = nullptr;
	ID3D12Resource* mTextureUploadHeap = nullptr;
    ID3D12Resource* mTextureResource;
    ID3D12Device* mDevice;
};
