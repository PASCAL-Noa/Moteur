#include "pch.h"
#include "SceneRender.h"
#include "../Engine/ECS/Components/InputComponent.h"
#include "../Engine/ECS/Systems/InputSystem.h"
#include "../Engine/ECS/Systems/CameraSystem.h"
#include "../Engine/ECS/ECS.h"
#include "../Render/MeshProperties.h"
#include "../Core/ConstantBuffer.h"
#include "../Engine/ECS/Systems/RenderSystem.h"
#include "../Engine/ECS/Systems/LightSystem.h"
#include "../Engine/ECS/Systems/PhysicsSystem.h"
#include "../Engine/ResourceManager.h"
using Microsoft::WRL::ComPtr;

SceneRender::SceneRender(HINSTANCE hInstance) : d3dApp(hInstance)
{

}


SceneRender::~SceneRender()
{
	if (mPSO) mPSO->Release();
	if (mRootSignature) mRootSignature->Release();
	if (mCbvHeap) mCbvHeap->Release();
	if (mvsByteCode) mvsByteCode->Release();
	if (mpsByteCode) mpsByteCode->Release();
}

bool SceneRender::Init()
{
	d3dApp::Initialize();
	mManager.SetScene(this);
	BuildDescriptorHeaps(100);


	MeshRenderer* meshRendererComponent = new MeshRenderer;
	mManager.AddSystem<RenderSystem>(&mManager);
	mManager.AddSystem<CameraSystem>(&mManager);
	mManager.AddSystem<InputSystem>(&mManager);
	mManager.AddSystem<LightSystem>(&mManager);
	mManager.AddSystem<PhysicsSystem>(&mManager);
	mDirectCmdListAlloc->Reset();
	mCommandList->Reset(mDirectCmdListAlloc, nullptr);
	BuildTexture(textureFilesCopy);
	mCommandList->Close();
	ID3D12CommandList* cmdsLists[] = { mCommandList };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	FlushCommandQueue();

	BuildConstantBuffers();

	BuildRootSignature();
	BuildShadersAndInputLayout();
	buildPSO();

	// Configuration de la scène et des entités

	mPlayer = &mManager.CreateEntity();

	mPlayer->transform.SetPosition(0.0f, 0.0f, -10.0f);
	mManager.AddComponent(*mPlayer, new CameraComponent());
	mManager.AddComponent(*mPlayer, new InputComponent());

	mManager.GetComponent<InputComponent>(*mPlayer).mKeyStates.clear();
	mCameraEntity = mManager.FindEntityWithComponent<CameraComponent>();
	FlushCommandQueue();

	return true;
}

void SceneRender::Draw()
{
	mDirectCmdListAlloc->Reset();
	mCommandList->Reset(mDirectCmdListAlloc, nullptr);

	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);

	CD3DX12_RESOURCE_BARRIER barrierToRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
		mSwapChainBuffer[mCurrBackBuffer], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	mCommandList->ResourceBarrier(1, &barrierToRenderTarget);

	FLOAT clearColor[] = { 0.f, 0.f, 0.f, 1.0f };
	mCommandList->ClearRenderTargetView(
		CD3DX12_CPU_DESCRIPTOR_HANDLE(mRtvHeap->GetCPUDescriptorHandleForHeapStart(), mCurrBackBuffer, mRtvDescriptorSize),
		clearColor, 0, nullptr);
	mCommandList->ClearDepthStencilView(
		mDsvHeap->GetCPUDescriptorHandleForHeapStart(),
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
		mRtvHeap->GetCPUDescriptorHandleForHeapStart(), mCurrBackBuffer, mRtvDescriptorSize);
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = mDsvHeap->GetCPUDescriptorHandleForHeapStart();
	mCommandList->OMSetRenderTargets(1, &rtvHandle, true, &dsvHandle);

	ID3D12DescriptorHeap* descriptorHeaps[] = { mSrvHeap };
	mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	mCommandList->SetGraphicsRootSignature(mRootSignature);
	mCommandList->SetPipelineState(mPSO);

	ResourceManager& resourceManager = ResourceManager::GetInstance();

	for (auto& entity : mManager.GetEntities()) {
		if (mManager.HasComponent<MeshRenderer>(*entity) && entity->IsAlive()) {
			MeshRenderer& meshRenderer = mManager.GetComponent<MeshRenderer>(*entity);

			CD3DX12_GPU_DESCRIPTOR_HANDLE texHandle(mSrvHeap->GetGPUDescriptorHandleForHeapStart());
			texHandle.Offset(meshRenderer.textureIndex, mCbvSrvDescriptorSize);



			auto passCB = mPassCB->Resource()->GetGPUVirtualAddress();
			auto objectCB = mManager.GetComponent<MeshRenderer>(*entity).mObjectCB->Resource()->GetGPUVirtualAddress();

			mCommandList->SetGraphicsRootConstantBufferView(3, passCB);
			mCommandList->SetGraphicsRootConstantBufferView(0, objectCB);

			texHandle.Offset(0, mCbvSrvDescriptorSize);
			mCommandList->SetGraphicsRootDescriptorTable(1, texHandle);

			if (meshRenderer.mObjectCB != nullptr) {
				mCommandList->SetGraphicsRootConstantBufferView(0, meshRenderer.mObjectCB->Resource()->GetGPUVirtualAddress());
			}
			else {
				// Gérer le cas où mObjectCB est nul
			}
			D3D12_VERTEX_BUFFER_VIEW vbv = meshRenderer.mMesh->VertexBufferView();
			D3D12_INDEX_BUFFER_VIEW ibv = meshRenderer.mMesh->IndexBufferView();
			mCommandList->IASetVertexBuffers(0, 1, &vbv);
			mCommandList->IASetIndexBuffer(&ibv);
			mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			mCommandList->DrawIndexedInstanced(meshRenderer.mMesh->DrawArgs[meshRenderer.mMesh->Name].IndexCount, 1, 0, 0, 0);
		}
	}
	CD3DX12_RESOURCE_BARRIER barrierToPresent = CD3DX12_RESOURCE_BARRIER::Transition(
		mSwapChainBuffer[mCurrBackBuffer], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	mCommandList->ResourceBarrier(1, &barrierToPresent);

	mCommandList->Close();
	ID3D12CommandList* cmdsLists[] = { mCommandList };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	mSwapChain->Present(1, 0);
	mCurrBackBuffer = 1 - mCurrBackBuffer;

	FlushCommandQueue();
}

void SceneRender::Update()
{

	UpdatePass();
	UpdateObject();
}

void SceneRender::UpdateObject() {
	for (auto& entity : mManager.GetEntities()) {
		if (mManager.HasComponent<MeshRenderer>(*entity) && entity->IsAlive()) {
			MeshRenderer& meshRenderer = mManager.GetComponent<MeshRenderer>(*entity);

			const CameraComponent& camera = mManager.GetComponent<CameraComponent>(*mCameraEntity);

			XMMATRIX world = entity->transform.GetMatrix();
			XMMATRIX viewMatrix = XMLoadFloat4x4(&camera.mView);
			XMMATRIX projMatrix = XMLoadFloat4x4(&camera.mProj);
			XMMATRIX worldViewProj = world * viewMatrix * projMatrix;

			ObjectConstants objConstants;
			XMStoreFloat4x4(&objConstants.WorldViewProj, XMMatrixTranspose(worldViewProj));
			XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(world));
			XMStoreFloat4x4(&objConstants.Projection, XMMatrixTranspose(projMatrix));
			XMStoreFloat4x4(&objConstants.View, XMMatrixTranspose(viewMatrix));

			meshRenderer.mObjectCB->CopyData(0, objConstants);
		}
	}
}


void SceneRender::UpdatePass() {
	Transform camera = mManager.GetComponent<CameraComponent>(*mCameraEntity).mCameraTransform;
	mMainPass.AmbientColor = XMFLOAT4{ 0.8f, 0.8f, 0.8f, 1.0f }; // Keep ambient light white
	mMainPass.EyePosW = XMFLOAT3(camera.GetPosition().x,
		camera.GetPosition().y,
		camera.GetPosition().z);
	mMainPass.light[0].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mMainPass.light[0].Color = XMFLOAT4(0.85f, 0.85f, 1.0f, 1.0f);
	mMainPass.light[0].Strength = XMFLOAT3(1.f, 1.f, 1.f);
	mMainPass.light[0].FalloffEnd = 30.0f;
	mMainPass.light[0].FalloffStart = 10.0f;
	mMainPass.light[0].SpotPower = 64.0f;

	XMVECTOR lightDir = -MathHelper::SphericalToCartesian(1.0f, 0.6 * XM_PI, 0.5 * XM_PI);
	XMStoreFloat3(&mMainPass.light[0].Direction, lightDir);


	mPassCB->CopyData(0, mMainPass);
}



void SceneRender::BuildTexture(std::vector < const wchar_t* > textureFiles)
{
	if (textureFilesCopy.size() > 1) {
		ResourceManager& resourceManager = resourceManager.GetInstance();

		resourceManager.SetDevice(md3dDevice);
		resourceManager.SetDescriptorSize(mCbvSrvDescriptorSize);

		for (size_t i = 0; i < textureFiles.size(); ++i) {
			resourceManager.LoadTexture(mCommandList, mCbvHeap, textureFiles[i], i);
		}
	}
	else {
		textureFilesCopy = textureFiles;
	}
}

void SceneRender::BuildShadersAndInputLayout()
{
	HRESULT hr = S_OK;
	ComPtr<ID3DBlob> errorBlob;


	mvsByteCode = d3dUtil::CompileShader(L"../../res/Render/Shaders/Default.hlsl", nullptr, "VS", "vs_5_0");
	if (!mvsByteCode)
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		}
		throw std::runtime_error("Failed to compile vertex shader");
	}

	// Compile the pixel shader
	mpsByteCode = d3dUtil::CompileShader(L"../../res/Render/Shaders/Default.hlsl", nullptr, "PS", "ps_5_0");
	if (!mpsByteCode)
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		}
		throw std::runtime_error("Failed to compile pixel shader");
	}

	mInputLayout =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,    0, 24,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

}

void SceneRender::buildPSO()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
	psoDesc.pRootSignature = mRootSignature;
	psoDesc.VS = { reinterpret_cast<BYTE*>(mvsByteCode->GetBufferPointer()), mvsByteCode->GetBufferSize() };
	psoDesc.PS = { reinterpret_cast<BYTE*>(mpsByteCode->GetBufferPointer()), mpsByteCode->GetBufferSize() };

	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // Disable backface culling

	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState.DepthEnable = true;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;

	psoDesc.BlendState.AlphaToCoverageEnable = false;
	psoDesc.BlendState.IndependentBlendEnable = false;
	psoDesc.BlendState.RenderTarget[0].BlendEnable = true;
	psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	psoDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	psoDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	psoDesc.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	psoDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = mBackBufferFormat;
	psoDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	psoDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	psoDesc.DSVFormat = mDepthStencilFormat;

	// Active la gestion des textures dans le pixel shader
	HRESULT hr = md3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSO));
}


void SceneRender::BuildDescriptorHeaps(std::size_t size)
{
	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
	cbvHeapDesc.NumDescriptors = size;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvHeapDesc.NodeMask = 0;
	md3dDevice->CreateDescriptorHeap(&cbvHeapDesc,
		IID_PPV_ARGS(&mCbvHeap));

	mCbvHeapOffset = 0;
}

void SceneRender::BuildConstantBuffers()
{

	mPassCB = new UploadBuffer<PassConstants>(md3dDevice, 1, true);

	//UINT passCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(PassConstants));

	//// Adresse GPU du PassBuffer
	//D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mPassCB->Resource()->GetGPUVirtualAddress();

	//// Création du Constant Buffer View pour la lumière
	//D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	//cbvDesc.BufferLocation = cbAddress;
	//cbvDesc.SizeInBytes = passCBByteSize;

	//// Récupérer la position correcte dans le heap
	//CD3DX12_CPU_DESCRIPTOR_HANDLE cbvHandle(mCbvHeap->GetCPUDescriptorHandleForHeapStart());
	//cbvHandle.Offset(mCbvHeapOffset, mCbvSrvDescriptorSize);

	//md3dDevice->CreateConstantBufferView(&cbvDesc, cbvHandle);

}

void SceneRender::BuildRootSignature()
{
	ResourceManager& resourceManager = resourceManager.GetInstance();
	resourceManager.SetDevice(md3dDevice);
	resourceManager.SetDescriptorSize(mCbvSrvDescriptorSize);

	CD3DX12_DESCRIPTOR_RANGE srvTable;
	srvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_ROOT_PARAMETER slotRootParameter[4];
	slotRootParameter[0].InitAsConstantBufferView(0);  // ObjectConstants (b0)
	slotRootParameter[1].InitAsDescriptorTable(1, &srvTable, D3D12_SHADER_VISIBILITY_PIXEL); // Texture (t0)
	slotRootParameter[2].InitAsConstants(4, 2, 0, D3D12_SHADER_VISIBILITY_PIXEL); // Change b1 -> b2 pour éviter le conflit
	slotRootParameter[3].InitAsConstantBufferView(1);  // PassConstants (b1)



	CD3DX12_STATIC_SAMPLER_DESC sampler(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR);

	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(4, slotRootParameter, 1, &sampler,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);


	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;

	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		&serializedRootSig, &errorBlob);

	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}

	md3dDevice->CreateRootSignature(0, serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&mRootSignature));

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = static_cast<UINT>(textureFilesCopy.size());
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	md3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&mSrvHeap));

	// Create Shader Resource Views (SRVs) for each texture
	CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(mSrvHeap->GetCPUDescriptorHandleForHeapStart());
	for (size_t i = 0; i < textureFilesCopy.size(); ++i) {
		ID3D12Resource* texture = resourceManager.GetTexture(textureFilesCopy[i]);
		if (!texture) continue;

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = texture->GetDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = texture->GetDesc().MipLevels;
		srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

		// Bind each texture to a unique descriptor slot
		md3dDevice->CreateShaderResourceView(texture, &srvDesc, srvHandle);
		srvHandle.Offset(1, mCbvSrvDescriptorSize);
	}

	isLoaded = true;
}


void SceneRender::PrintMatrix(const XMMATRIX& matrix)
{
	XMFLOAT4X4 m;
	XMStoreFloat4x4(&m, matrix);

	std::cout << "XMMATRIX:\n";
	for (int i = 0; i < 4; ++i)
	{
		std::cout << m.m[i][0] << " " << m.m[i][1] << " " << m.m[i][2] << " " << m.m[i][3] << "\n";
	}
	std::cout << std::endl;
}
