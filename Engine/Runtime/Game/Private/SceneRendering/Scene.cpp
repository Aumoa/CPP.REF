// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/Scene.h"

#include "Engine.h"
#include "World.h"
#include "GameInstance.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"
#include "DirectX/DirectXAccelerationInstancingScene.h"
#include "DirectX/DirectXShaderBindingTable.h"
#include "DirectX/DirectXDescriptorAllocator.h"
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
	localPlayerVisibility = NewObject<SceneVisibility>(this);
	DirectXDeviceBundle* deviceBundle = engine->GetDeviceBundle();
	DirectXNew(instancingScene, DirectXAccelerationInstancingScene, deviceBundle);
	DirectXNew(sbt, DirectXShaderBindingTable, deviceBundle);
	DirectXNew(allocator, DirectXDescriptorAllocator, deviceBundle);
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
			if (sceneProxies[i] != nullptr)
			{
				sceneProxies[i]->PrimitiveId = i;
				instancingScene->AddInstanceDeferred(i, sceneProxies[i]);
			}
		}
	}
	instancingScene->FinishEditInstance();

	for (size_t i = 0; i < lightComponents.size(); ++i)
	{
		GLightComponent*& primitive = lightComponents[i];

		if (primitive->HasAnyDirtyMark())
		{
			primitive->ResolveDirtyState();
			lightProxies[i] = primitive->GetSceneProxy();
		}
	}

	instancingScene->UpdateScene();
}

void Scene::InitViews()
{
	GPlayerCameraManager* cameraManager = localPlayer->CameraManager;

	if (cameraManager == nullptr)
	{
		SE_LOG(LogRendering, Error, L"PlayerController have not a camera manager component. Abort.");
		return;
	}

	// Calc scene visibilities for each views.
	MinimalViewInfo viewInfo;
	cameraManager->CalcCameraView(viewInfo);

	localPlayerVisibility->UpdateView(viewInfo);
	localPlayerVisibility->CalcVisibility();

	// Calc count of shader resource views.
	numSRVs = 256;  // Reserved
	numSRVs += localPlayerVisibility->NumPrimitivesRender * 2;  // Material
}

void Scene::BeginRender(DirectXDeviceContext* deviceContext)
{
	instancingScene->BuildScene(deviceContext);
	allocator->BeginAllocate((uint32)numSRVs);
}

void Scene::EndRender(DirectXDeviceContext* deviceContext)
{
	allocator->EndAllocate();
}

void Scene::AddPrimitive(GPrimitiveComponent* inPrimitiveComponent)
{
	size_t index = primitiveComponents.size();
	PrimitiveSceneProxy* sceneProxy = inPrimitiveComponent->GetSceneProxy();

	primitiveComponents.emplace_back(inPrimitiveComponent);
	sceneProxies.emplace_back(sceneProxy);

	if (sceneProxy != nullptr)
	{
		sceneProxy->PrimitiveId = index;
		instancingScene->AddInstance(index, sceneProxy);
	}
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

Engine* Scene::GetEngine() const
{
	return engine;
}

DirectXAccelerationInstancingScene* Scene::GetAccelScene() const
{
	return instancingScene.Get();
}

DirectXShaderBindingTable* Scene::GetShaderBindingTable() const
{
	return sbt.Get();
}

DirectXDescriptorAllocator* Scene::GetDescriptorAllocator() const
{
	return allocator.Get();
}