#include "pch.h"
#include "Texture.h"
#include "DDSTextureLoader.h"

TextureLoader::TextureLoader() {}

void TextureLoader::SetDevice(ID3D12Device* device)
{
    mDevice = device;
}

void TextureLoader::LoadTexture(ID3D12GraphicsCommandList* cmdList, ID3D12DescriptorHeap* srvHeap, const wchar_t* filename)
{
	if (mTexture != nullptr)
	{
		mTexture->Release();
		mTexture = nullptr;
	}
	if (mTextureUploadHeap != nullptr)
	{
		mTextureResource->Release();
		mTextureResource = nullptr;
	}

    DirectX::CreateDDSTextureFromFile12(
        mDevice,
        cmdList,
        filename,
        &mTexture,
        &mTextureUploadHeap
    );

    mTextureResource = mTexture;

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Format = mTextureResource->GetDesc().Format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = mTextureResource->GetDesc().MipLevels;

    CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(srvHeap->GetCPUDescriptorHandleForHeapStart());
    mDevice->CreateShaderResourceView(mTextureResource, &srvDesc, srvHandle);
}

