#include "pch.h"
#include "RenderSystem.h"
#include "../Engine/ECS/Components/MeshRenderer.h"
#include "../Render/SceneRender.h"
#include "d3dcompiler.h"

RenderSystem::RenderSystem(Manager* m) : manager(m){}

RenderSystem::~RenderSystem()
{
    
}



void RenderSystem::Init()
{
    for (auto& entity : manager->GetEntities())
    {
        if (manager->HasComponent<MeshRenderer>(*entity) && entity->IsAlive())
        {
            auto& meshCompo = manager->GetComponent<MeshRenderer>(*entity);
            meshCompo.mMeshRendererTransform = entity->transform;
            meshCompo.x = entity->transform.GetPosition().x;
            meshCompo.y = entity->transform.GetPosition().y;
            meshCompo.z = entity->transform.GetPosition().z;
            meshCompo.mMeshRendererTransform.SetPosition(meshCompo.x, meshCompo.y, meshCompo.z);
            meshCompo.mMeshRendererTransform.SetRotation(entity->transform.GetRotation().x, entity->transform.GetRotation().y, entity->transform.GetRotation().z);
        }
    }
}

void RenderSystem::Update(float deltaTime)
{
    for (auto& entity : manager->GetEntities())
    {
        if (manager->HasComponent<MeshRenderer>(*entity) && entity->IsAlive())
        {
            auto& meshCompo = manager->GetComponent<MeshRenderer>(*entity);
            meshCompo.mMeshRendererTransform = entity->transform;
            meshCompo.x = entity->transform.GetPosition().x;
            meshCompo.y = entity->transform.GetPosition().y;
            meshCompo.z = entity->transform.GetPosition().z;
            meshCompo.mMeshRendererTransform.SetPosition(meshCompo.x, meshCompo.y, meshCompo.z);
            meshCompo.mMeshRendererTransform.SetRotation(entity->transform.GetRotation().x, entity->transform.GetRotation().y, entity->transform.GetRotation().z);
            
        }
    }
}

bool RenderSystem::HasTexture(Entity* entity) const
{
    return manager->GetComponent<MeshRenderer>(*entity).mTexture != nullptr;;
}

void RenderSystem::SetTexture(Entity* entity, ID3D12Resource* texture) {
    if (manager->HasComponent<MeshRenderer>(*entity) && entity->IsAlive())
    {
        auto& meshCompo = manager->GetComponent<MeshRenderer>(*entity);
        meshCompo.mTexture = texture;
    }
}

void RenderSystem::AddMesh(Entity* entity, const std::string& name, MeshProperties mesh)
{
    if (manager->HasComponent<MeshRenderer>(*entity) && entity->IsAlive())
    {
        auto& meshCompo = manager->GetComponent<MeshRenderer>(*entity);
        SceneRender* scene = entity->GetScene();

        ID3D12Device* d3dDevice = scene->GetD3DDevice();
        ID3D12GraphicsCommandList* commandList = scene->GetCommandList();
        ID3D12DescriptorHeap* cbvHeap = scene->GetCbvHeap();
        UINT cbvHeapOffset = scene->GetCbvHeapOffset();
        UINT cbvSrvDescriptorSize = scene->GetCbvSrvDescriptorSize();
        ID3D12CommandAllocator* directCmdListAlloc = scene->GetDirectCmdListAlloc();
        ID3D12CommandQueue* commandQueue = scene->GetCommandQueue();

        directCmdListAlloc->Reset();
        commandList->Reset(directCmdListAlloc, nullptr);

        meshCompo.mMesh = new MeshGeometry();
        meshCompo.mMesh->Name = name;
        meshCompo.mMesh->cbvIndex = cbvHeapOffset;
        BuildConstantBuffer(entity, d3dDevice, cbvHeap, cbvHeapOffset, cbvSrvDescriptorSize);
       
        D3DCreateBlob(mesh.vbByteSize, &meshCompo.mMesh->VertexBufferCPU);
        CopyMemory(meshCompo.mMesh->VertexBufferCPU->GetBufferPointer(), mesh.vertices.data(), mesh.vbByteSize);

        D3DCreateBlob(mesh.ibByteSize, &meshCompo.mMesh->IndexBufferCPU);
        CopyMemory(meshCompo.mMesh->IndexBufferCPU->GetBufferPointer(), mesh.indices.data(), mesh.ibByteSize);

        meshCompo.mMesh->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(d3dDevice, commandList,
            mesh.vertices.data(), mesh.vbByteSize,
            meshCompo.mMesh->VertexBufferUploader);
        meshCompo.mMesh->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(d3dDevice, commandList,
            mesh.indices.data(), mesh.ibByteSize,
            meshCompo.mMesh->IndexBufferUploader);

        meshCompo.mMesh->VertexByteStride = sizeof(MeshProperties::Vertex);
        meshCompo.mMesh->VertexBufferByteSize = mesh.vbByteSize;
        meshCompo.mMesh->IndexFormat = DXGI_FORMAT_R32_UINT;
        meshCompo.mMesh->IndexBufferByteSize = mesh.ibByteSize;

        // Création du submesh
        SubmeshGeometry submesh;
        submesh.IndexCount = (UINT)mesh.indices.size();
        submesh.StartIndexLocation = 0;
        submesh.BaseVertexLocation = 0;

        meshCompo.mMesh->DrawArgs[name] = submesh;
        meshCompo.mMesh->geoTransform = mesh.mMeshTransform;

        commandList->Close();
        ID3D12CommandList* cmdsLists[] = { commandList };
        commandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
        entity->GetScene()->FlushCommandQueue();
    }
}


void RenderSystem::BuildConstantBuffer(Entity* entity, ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, UINT cbvHeapOffset, UINT CbvSrvDescriptorSize)
{
    auto& meshCompo = manager->GetComponent<MeshRenderer>(*entity);
    meshCompo.mObjectCB = new UploadBuffer<ObjectConstants>(device, 1, true);


    UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));

    // Adresse GPU du Constant Buffer
    D3D12_GPU_VIRTUAL_ADDRESS cbAddress = meshCompo.mObjectCB->Resource()->GetGPUVirtualAddress();

    // Créer la description du CBV
    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
    cbvDesc.BufferLocation = cbAddress;
    cbvDesc.SizeInBytes = objCBByteSize;

    // Récupérer la position correcte dans le heap
    CD3DX12_CPU_DESCRIPTOR_HANDLE cbvHandle(cbvHeap->GetCPUDescriptorHandleForHeapStart());
    cbvHandle.Offset(cbvHeapOffset, CbvSrvDescriptorSize);

    device->CreateConstantBufferView(&cbvDesc, cbvHandle);
}


