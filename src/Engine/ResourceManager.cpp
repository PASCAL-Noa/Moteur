#include "pch.h"
#include "ResourceManager.h"
#include "DDSTextureLoader.h"

ResourceManager& ResourceManager::GetInstance() {
    static ResourceManager instance;
    return instance;
}

void ResourceManager::SetDevice(ID3D12Device* device) {
    mDevice = device;
}

// ------------------ TEXTURE LOADING ------------------

ID3D12Resource* ResourceManager::LoadTexture(ID3D12GraphicsCommandList* cmdList, ID3D12DescriptorHeap* srvHeap, const wchar_t* filename, UINT index) {

    if (textureCache.find(filename) != textureCache.end()) {
        return GetTexture(filename);
    }

    // Load the texture

    if (textureCache.find(filename) != textureCache.end()) {
        return GetTexture(filename);
    }

    HRESULT result = DirectX::CreateDDSTextureFromFile12(
        mDevice,
        cmdList,
        filename,
        &mTexture,
        &mTextureUploadHeap
    );

    if (FAILED(result) || mTexture == nullptr) {
        throw std::runtime_error("Failed to load texture");
    }

    mTextureRessource = mTexture;

    // Create Shader Resource View
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Format = mTextureRessource->GetDesc().Format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = mTextureRessource->GetDesc().MipLevels;

    CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(srvHeap->GetCPUDescriptorHandleForHeapStart(), index, mCbvSrvDescriptorSize);
    mDevice->CreateShaderResourceView(mTextureRessource, &srvDesc, srvHandle);

    textureCache[filename] = mTexture;

	return mTexture;
}

ID3D12Resource* ResourceManager::GetTexture(const std::wstring& filename) {
    if (textureCache.find(filename) != textureCache.end()) {
        return textureCache[filename];
    }
    return nullptr;
}

// ------------------ MESH LOADING ------------------

std::shared_ptr<MeshProperties> ResourceManager::LoadMesh(const std::string& filename) {
    if (meshCache.find(filename) != meshCache.end()) {
        return meshCache[filename];
    }

    std::shared_ptr<MeshProperties> mesh = std::make_shared<MeshProperties>(MeshProperties::CreateCustomMesh(1.0f, filename));

    if (!mesh) {
        throw std::runtime_error("Failed to load mesh: " + filename);
    }

    meshCache[filename] = mesh;
    return mesh;
}

std::shared_ptr<MeshProperties> ResourceManager::GetMesh(const std::string& filename) {
    if (meshCache.find(filename) != meshCache.end()) {
        return meshCache[filename];
    }
    return nullptr;
}
