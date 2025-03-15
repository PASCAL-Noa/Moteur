#pragma once
#include "../ECS.h"
#include "../Render/d3dUtil.h"
#include "../Core/ConstantBuffer.h"
#include "../Render/UploadBuffer.h"
#include "../Core/Tools/Transform.h"

class MeshProperties;

struct MeshRenderer : public Component
{
	
	MeshGeometry* mMesh;
	float x, y;
	float z;
	ID3D12Resource* mTexture;
	Transform mMeshRendererTransform;
	UploadBuffer<ObjectConstants>* mObjectCB = nullptr;

	UINT textureIndex;
	std::string mName;

	~MeshRenderer()
	{
		if (mMesh)
		{
			mMesh->VertexBufferCPU->Release();
			mMesh->IndexBufferCPU->Release();

			mMesh->VertexBufferGPU->Release();
			mMesh->IndexBufferGPU->Release();

			mMesh->VertexBufferUploader->Release();
			mMesh->IndexBufferUploader->Release();

			mMesh = nullptr;
			delete mMesh;
		}

		if (mObjectCB)
		{
			mObjectCB->Resource()->Release();
		}
	}
};

