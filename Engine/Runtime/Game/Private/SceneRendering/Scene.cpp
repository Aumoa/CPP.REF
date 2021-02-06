// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/Scene.h"

#include "Components/PrimitiveComponent.h"
#include "Components/PlayerCameraManager.h"
#include "Components/LightComponent.h"
#include "Framework/PlayerController.h"
#include "SceneRendering/PrimitiveSceneProxy.h"
#include "SceneRendering/SceneVisibility.h"
#include "SceneRendering/LightSceneProxy.h"
#include "Logging/LogMacros.h"

using namespace std;

Scene::Scene() : Super()
	, localPlayer(nullptr)
{
	localPlayerVisibility = NewObject<SceneVisibility>(this);
}

Scene::~Scene()
{

}

void Scene::Update()
{
	for (size_t i = 0; i < primitiveComponents.size(); ++i)
	{
		PrimitiveComponent*& primitive = primitiveComponents[i];
		PrimitiveSceneProxy*& sceneProxy = sceneProxies[i];

		if (primitive->HasDirtyMark())
		{
			primitive->ResolveDirtyState();
			sceneProxy = primitive->GetSceneProxy();
			sceneProxy->UpdateMovable();
		}
	}

	for (size_t i = 0; i < lightComponents.size(); ++i)
	{
		LightComponent*& primitive = lightComponents[i];
		LightSceneProxy*& sceneProxy = lightProxies[i];

		if (primitive->HasDirtyMark())
		{
			primitive->ResolveDirtyState();
			sceneProxy = primitive->GetSceneProxy();
			sceneProxy->UpdateMovable();
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