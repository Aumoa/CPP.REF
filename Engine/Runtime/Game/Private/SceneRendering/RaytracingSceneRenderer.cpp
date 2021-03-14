// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/RaytracingSceneRenderer.h"

#include "Engine.h"
#include "GameViewport.h"
#include "SceneRendering/SceneVisibility.h"
#include "SceneRendering/PrimitiveSceneProxy.h"
#include "SceneRendering/Scene.h"
#include "Assets/CachedShaderLibrary.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/DirectXRaytracingShader.h"
#include "DirectX/DirectXDescriptorAllocator.h"
#include "DirectX/DirectXShaderResourceView.h"
#include "DirectX/DirectXShaderBindingTable.h"
#include "DirectX/DirectXAccelerationInstancingScene.h"
#include "Logging/LogMacros.h"

using namespace std;

RaytracingSceneRenderer::RaytracingSceneRenderer(Scene* inScene) : Super(inScene)
	, shaderLibrary(nullptr)
	, globalInputs{ }
{
	shaderLibrary = inScene->GetEngine()->GetCachedShaderLibrary();
}

RaytracingSceneRenderer::~RaytracingSceneRenderer()
{

}

void RaytracingSceneRenderer::RenderScene(ID3D12GraphicsCommandList4* inCommandList, GameViewport* inViewport)
{
	Scene* scene = GetScene();

	DirectXRaytracingShader* cached = shaderLibrary->GetOrReadyShader();
	cached->Render(inCommandList);

	DirectXDescriptorAllocator* allocator = scene->GetDescriptorAllocator();
	DirectXShaderBindingTable* sbt = scene->GetShaderBindingTable();

	D3D12_DISPATCH_RAYS_DESC dispatchRays = { };
	sbt->Init(cached);
	sbt->FillDispatchRaysDesc(dispatchRays);
	dispatchRays.Width = inViewport->ResolutionX;
	dispatchRays.Height = inViewport->ResolutionY;
	dispatchRays.Depth = 1;

	SceneVisibility* localVisibility = scene->GetLocalPlayerVisibility();
	allocator->SetDescriptorHeaps(inCommandList);
	inCommandList->SetComputeRootConstantBufferView(0, localVisibility->GetCameraConstantBuffer());
	inCommandList->SetComputeRootDescriptorTable(1, allocator->GetGPUHandle(allocator->Issue(globalInputs.ColorOutput)));
	inCommandList->SetComputeRootShaderResourceView(2, scene->GetAccelScene()->GetRaytracingAccelerationStructureBuffer());
	inCommandList->DispatchRays(&dispatchRays);
}

void RaytracingSceneRenderer::InitGlobalInputs(const ShaderGlobalInputs& globalInputs)
{
	this->globalInputs = globalInputs;
}