#pragma once
#include "Texture.h"
#include "MeshProperties.h"

class ResourceManager {
public:
    static ResourceManager& GetInstance();

    void SetDevice(ID3D12Device* device);
    void SetDescriptorSize(UINT descriptorSize) {
        mCbvSrvDescriptorSize = descriptorSize;
    }

    // Texture Management
    ID3D12Resource* LoadTexture(ID3D12GraphicsCommandList* cmdList, ID3D12DescriptorHeap* srvHeap, const wchar_t* filename, UINT index);
    ID3D12Resource* GetTexture(const std::wstring& filename);

    // Mesh Management
    std::shared_ptr<MeshProperties> LoadMesh(const std::string& filename);
    std::shared_ptr<MeshProperties> GetMesh(const std::string& filename);

private:
    ResourceManager() = default;
    ~ResourceManager() = default;

    std::unordered_map<std::wstring, ID3D12Resource*> textureCache;
    std::unordered_map<std::string, std::shared_ptr<MeshProperties>> meshCache;

    ID3D12Resource* mTexture = nullptr;
    ID3D12Resource* mTextureRessource;
    ID3D12Resource* mTextureUploadHeap = nullptr;
    ID3D12Device* mDevice = nullptr;
    ID3D12GraphicsCommandList* mCommandList = nullptr;
    ID3D12DescriptorHeap* mSrvHeap = nullptr;
    UINT mCbvSrvDescriptorSize = 0;
};
