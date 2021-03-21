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
#include "Diagnostics/ScopedCycleCounter.h"

using namespace std;

DEFINE_STATS_GROUP(RaytracingSceneRenderer);

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
	QUICK_SCOPED_CYCLE_COUNTER(RaytracingSceneRenderer, RenderScene);

	Scene* scene = GetScene();

	DirectXRaytracingShader* cached = shaderLibrary->GetOrReadyShader();
	cached->Render(inCommandList);

	DirectXDescriptorAllocator* allocator = scene->DescriptorAllocator;
	DirectXShaderBindingTable* sbt = scene->ShaderBindingTable;

	// Fill instanced hit group records.
	static thread_local std::vector<DirectXInstanceShaderRecord> hitGroupRecords;
	static thread_local std::vector<DirectXInstanceShaderRecord> missRecords;
	
	const auto& primitives = scene->GetPrimitives();

	// Calc count of shader records.
	size_t shaderRecords = 0;
	for (auto& primitive : primitives)
	{
		shaderRecords += primitive->InstanceShaderRecord.size();
	}

	// Assign shader records.
	hitGroupRecords.resize(0);
	hitGroupRecords.reserve(shaderRecords);
	for (auto& primitive : primitives)
	{
		for (size_t i = 0; i < primitive->InstanceShaderRecord.size(); ++i)
		{
			auto record = primitive->InstanceShaderRecord[i];
			for (auto app : primitive->DeferredShaderRecords[i].ShaderRecordApps)
			{
				record.RootParameters.emplace_back(allocator->GetGPUHandle(allocator->Issue(app)).ptr);
			}
			hitGroupRecords.emplace_back(record);
		}
	}

	// Fill instanced miss records.
	// In most cases, this is may be sky sphere.
	missRecords.resize(0);
	missRecords.emplace_back(0);

	sbt->Init(cached, hitGroupRecords, missRecords);

	D3D12_DISPATCH_RAYS_DESC dispatchRays = { };
	sbt->FillDispatchRaysDesc(dispatchRays);
	dispatchRays.Width = inViewport->ResolutionX;
	dispatchRays.Height = inViewport->ResolutionY;
	dispatchRays.Depth = 1;

	SceneVisibility* localVisibility = scene->GetLocalPlayerVisibility();
	allocator->SetDescriptorHeaps(inCommandList);
	inCommandList->SetComputeRootConstantBufferView(0, localVisibility->GetCameraConstantBuffer());
	inCommandList->SetComputeRootDescriptorTable(1, allocator->GetGPUHandle(allocator->Issue(globalInputs.ColorOutput)));
	inCommandList->SetComputeRootDescriptorTable(2, allocator->GetGPUHandle(allocator->Issue(scene->SceneSRV)));
	inCommandList->DispatchRays(&dispatchRays);
}

void RaytracingSceneRenderer::InitGlobalInputs(const ShaderGlobalInputs& globalInputs)
{
	this->globalInputs = globalInputs;
}