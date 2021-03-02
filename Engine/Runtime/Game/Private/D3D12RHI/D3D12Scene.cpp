// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12Scene.h"

#include "Engine.h"
#include "D3D12DeviceBundle.h"
#include "D3D12ResourceBundle.h"
#include "D3D12CommandList.h"
#include "Components/PrimitiveComponent.h"
#include "Components/PlayerCameraManager.h"
#include "Components/LightComponent.h"
#include "Framework/PlayerController.h"
#include "SceneRendering/PrimitiveSceneProxy.h"
#include "SceneRendering/SceneVisibility.h"
#include "SceneRendering/LightSceneProxy.h"
#include "Logging/LogMacros.h"
#include "RHI/IRHIOnlineDescriptorPatch.h"
#include "RHI/IRHIResource.h"

using namespace std;

D3D12Scene::D3D12Scene(APlayerController* inPlayerController) : Super()
	, deviceBundle(nullptr)
	, resourceBundle(nullptr)

	, localPlayer(inPlayerController)
	, numSRVs(0)
{
	deviceBundle = Cast<D3D12DeviceBundle>(GEngine.DeviceBundle);
	localPlayerVisibility = NewObject<SceneVisibility>(this);
	srvPatch = deviceBundle->CreateOnlineDescriptorPatch();
}

D3D12Scene::~D3D12Scene()
{

}

void D3D12Scene::Update()
{
	for (size_t i = 0; i < primitiveComponents.size(); ++i)
	{
		PrimitiveComponent*& primitive = primitiveComponents[i];

		if (primitive->HasAnyDirtyMark())
		{
			primitive->ResolveDirtyState();
			sceneProxies[i] = primitive->GetSceneProxy();
		}
	}

	for (size_t i = 0; i < lightComponents.size(); ++i)
	{
		LightComponent*& primitive = lightComponents[i];

		if (primitive->HasAnyDirtyMark())
		{
			primitive->ResolveDirtyState();
			lightProxies[i] = primitive->GetSceneProxy();
		}
	}
}

void D3D12Scene::CalcVisibility()
{
	PlayerCameraManager* cameraManager = localPlayer->CameraManager;

	if (cameraManager == nullptr)
	{
		SE_LOG(LogRendering, Error, L"PlayerController have not a camera manager component. Abort.");
		return;
	}

	MinimalViewInfo viewInfo;
	cameraManager->CalcCameraView(viewInfo);

	localPlayerVisibility->UpdateView(viewInfo);
	localPlayerVisibility->CalcVisibility();

	numSRVs = 256;  // Reserved
	numSRVs += localPlayerVisibility->NumPrimitivesRender * 2;  // Material
	
}

void D3D12Scene::BeginRender(IRHICommandList* inCommandList)
{
	srvPatch->ReallocAndStart(numSRVs);
	inCommandList->SetShaderDescriptorPatch(srvPatch.Get());

	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC buildDesc = { };
	buildDesc.DestAccelerationStructureData = topLevelAS->GetVirtualAddress();

	auto cmdList = Cast<D3D12CommandList>(inCommandList);
}

void D3D12Scene::EndRender(IRHICommandList* inCommandList)
{

}

void D3D12Scene::AddPrimitive(PrimitiveComponent* inPrimitiveComponent)
{
	primitiveComponents.emplace_back(inPrimitiveComponent);
	sceneProxies.emplace_back(inPrimitiveComponent->GetSceneProxy());

	ReadyRaytracingAccelerationBuffers();
}

void D3D12Scene::AddLight(LightComponent* inLightComponent)
{
	lightComponents.emplace_back(inLightComponent);
	lightProxies.emplace_back(inLightComponent->GetSceneProxy());
}

SceneVisibility* D3D12Scene::GetLocalPlayerVisibility() const
{
	return localPlayerVisibility.Get();
}

span<PrimitiveSceneProxy* const> D3D12Scene::GetPrimitives() const
{
	return sceneProxies;
}

span<LightSceneProxy* const> D3D12Scene::GetLights() const
{
	return lightProxies;
}

void D3D12Scene::ReadyRaytracingAccelerationBuffers()
{
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs = { };
	inputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;
	inputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;
	inputs.NumDescs = (uint32)primitiveComponents.size();
	inputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_UPDATE;

	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO prebuild = { };
	deviceBundle->Device->GetRaytracingAccelerationStructurePrebuildInfo(&inputs, &prebuild);

	topLevelAS = resourceBundle->CreateRaytracingAccelerationStructureBuffer(prebuild.ResultDataMaxSizeInBytes);
	topLevelScratch = resourceBundle->CreateRaytracingScratchBuffer(prebuild.ScratchDataSizeInBytes);
}