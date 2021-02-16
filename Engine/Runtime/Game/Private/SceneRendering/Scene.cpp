// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/Scene.h"

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

Scene::Scene() : Super()
	, localPlayer(nullptr)
	, numSRVs(2048)
{
	localPlayerVisibility = NewObject<SceneVisibility>(this);
	srvPatch = GEngine.DeviceBundle->CreateOnlineDescriptorPatch();
}

Scene::~Scene()
{

}

void Scene::Update()
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

void Scene::CalcVisibility()
{
	APlayerController* localPlayer = LocalPlayer;
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

void Scene::BeginRender(IRHICommandList* inCommandList)
{
	srvPatch->ReallocAndStart(numSRVs);
	inCommandList->SetShaderDescriptorPatch(srvPatch.Get());
}

void Scene::EndRender(IRHICommandList* inCommandList)
{

}

void Scene::AddScene(PrimitiveComponent* inPrimitiveComponent)
{
	primitiveComponents.emplace_back(inPrimitiveComponent);
	sceneProxies.emplace_back(inPrimitiveComponent->GetSceneProxy());
}

void Scene::AddLight(LightComponent* inLightComponent)
{
	lightComponents.emplace_back(inLightComponent);
	lightProxies.emplace_back(inLightComponent->GetSceneProxy());
}

SceneVisibility* Scene::GetLocalPlayerVisibility() const
{
	return localPlayerVisibility.Get();
}

APlayerController* Scene::LocalPlayer_get() const
{
	return localPlayer;
}

void Scene::LocalPlayer_set(APlayerController* value)
{
	localPlayer = value;
}