#pragma once
#include "../ECS.h"
#include "../Render/MeshProperties.h"
#include "../Render/d3dUtil.h"

class RenderSystem : public System
{
private:
	Manager* manager;
public:
	RenderSystem(Manager* m);
	~RenderSystem();
	void Init()override;
	void Update(float deltaTime)override;

	void SetTexture(Entity* entity, ID3D12Resource* texture);
	bool HasTexture(Entity* entity)const;
	void AddMesh(Entity* entity, const std::string& name, MeshProperties mesh);
	void BuildConstantBuffer(Entity* entity, ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, UINT cbvHeapOffset, UINT mCbvSrvDescriptorSize);
	void SetScene(Manager* m) { manager = m; }
};
