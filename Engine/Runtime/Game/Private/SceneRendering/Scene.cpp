// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/Scene.h"

#include "Engine.h"
#include "World.h"
#include "GameInstance.h"
#include "Components/PrimitiveComponent.h"
#include "Components/PlayerCameraManager.h"
#include "Components/LightComponent.h"
#include "Framework/PlayerController.h"
#include "SceneRendering/PrimitiveSceneProxy.h"
#include "SceneRendering/SceneVisibility.h"
#include "SceneRendering/LightSceneProxy.h"
#include "Logging/LogMacros.h"

using namespace std;

Scene::Scene(APlayerController* inPlayerController) : Super()
	, engine(inPlayerController->GetWorld()->GetGameInstance()->GetEngine())

	, localPlayer(inPlayerController)
	, numSRVs(0)
{

}

Scene::~Scene()
{

}

void Scene::Update()
{
	for (size_t i = 0; i < primitiveComponents.size(); ++i)
	{
		GPrimitiveComponent*& primitive = primitiveComponents[i];

		if (primitive->HasAnyDirtyMark())
		{
			primitive->ResolveDirtyState();
			sceneProxies[i] = primitive->GetSceneProxy();
		}
	}

	for (size_t i = 0; i < lightComponents.size(); ++i)
	{
		GLightComponent*& primitive = lightComponents[i];

		if (primitive->HasAnyDirtyMark())
		{
			primitive->ResolveDirtyState();
			lightProxies[i] = primitive->GetSceneProxy();
		}
	}
}

void Scene::CalcVisibility()
{
	GPlayerCameraManager* cameraManager = localPlayer->CameraManager;

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

}

void Scene::EndRender(IRHICommandList* inCommandList)
{

}

void Scene::AddPrimitive(GPrimitiveComponent* inPrimitiveComponent)
{
	primitiveComponents.emplace_back(inPrimitiveComponent);
	sceneProxies.emplace_back(inPrimitiveComponent->GetSceneProxy());
}

void Scene::AddLight(GLightComponent* inLightComponent)
{
	lightComponents.emplace_back(inLightComponent);
	lightProxies.emplace_back(inLightComponent->GetSceneProxy());
}

SceneVisibility* Scene::GetLocalPlayerVisibility() const
{
	return localPlayerVisibility.Get();
}

span<PrimitiveSceneProxy* const> Scene::GetPrimitives() const
{
	return sceneProxies;
}

span<LightSceneProxy* const> Scene::GetLights() const
{
	return lightProxies;
}