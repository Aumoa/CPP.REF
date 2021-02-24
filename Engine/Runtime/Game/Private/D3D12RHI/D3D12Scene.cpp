// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12Scene.h"

#include "Engine.h"
#include "Components/PrimitiveComponent.h"
#include "Components/PlayerCameraManager.h"
#include "Components/LightComponent.h"
#include "Framework/PlayerController.h"
#include "SceneRendering/PrimitiveSceneProxy.h"
#include "SceneRendering/SceneVisibility.h"
#include "SceneRendering/LightSceneProxy.h"
#include "Logging/LogMacros.h"
#include "RHI/IRHIOnlineDescriptorPatch.h"
#include "RHI/IRHIDeviceBundle.h"
#include "RHi/IRHICommandList.h"

using namespace std;

D3D12Scene::D3D12Scene(APlayerController* inPlayerController) : Super()
	, localPlayer(inPlayerController)
	, numSRVs(0)
{
	localPlayerVisibility = NewObject<SceneVisibility>(this);
	srvPatch = GEngine.DeviceBundle->CreateOnlineDescriptorPatch();
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
}

void D3D12Scene::EndRender(IRHICommandList* inCommandList)
{

}

void D3D12Scene::AddPrimitive(PrimitiveComponent* inPrimitiveComponent)
{
	primitiveComponents.emplace_back(inPrimitiveComponent);
	sceneProxies.emplace_back(inPrimitiveComponent->GetSceneProxy());
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