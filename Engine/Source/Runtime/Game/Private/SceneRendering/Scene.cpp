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
#include "DirectX/DirectXInstancedBufferAllocator.h"
#include "DirectX/DirectXShaderResourceView.h"
#include "Components/PrimitiveComponent.h"
#include "Components/PlayerCameraManager.h"
#include "Components/LightComponent.h"
#include "Framework/PlayerController.h"
#include "SceneRendering/PrimitiveSceneProxy.h"
#include "SceneRendering/SceneVisibility.h"
#include "SceneRendering/LightSceneProxy.h"
#include "Logging/LogMacros.h"
#include "Materials/ShaderManager.h"

using namespace std;

Scene::Scene(APlayerController* inPlayerController) : Super()
	, engine(inPlayerController->GetWorld()->GetGameInstance()->GetEngine())

	, localPlayer(inPlayerController)
	, numSRVs(0)
{
	localPlayerVisibility = NewObject<SceneVisibility>(this);
	DirectXDeviceBundle* deviceBundle = engine->GetDeviceBundle();
	DirectXNew(sceneSRV, DirectXShaderResourceView, deviceBundle, 2);
	DirectXNew(instancingScene, DirectXAccelerationInstancingScene, deviceBundle);
	DirectXNew(sbt, DirectXShaderBindingTable, deviceBundle);
	DirectXNew(allocator, DirectXDescriptorAllocator, deviceBundle);
	DirectXNew(lightInstanced, DirectXInstancedBufferAllocator, deviceBundle, sizeof(HomogeneousLight));
	
	shaderManager = NewObject<ShaderManager>(engine);
}

Scene::~Scene()
{

}

void Scene::Update()
{
	const uint64 prevBuffer = instancingScene->GetRaytracingAccelerationStructureBuffer();

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

				for (auto material : sceneProxies[i]->Materials)
				{
					shaderManager->AddMaterialRef(material);
				}
			}
		}
	}
	instancingScene->FinishEditInstance();
	instancingScene->UpdateScene();

	const bool bInstancingBufferUpdated = prevBuffer != instancingScene->GetRaytracingAccelerationStructureBuffer();
	const bool bLightCountUpdated = lightInstanced->Count != lightComponents.size();

	lightInstanced->Count = lightComponents.size();
	static HomogeneousLight nullLight = { };
	for (size_t i = 0; i < lightComponents.size(); ++i)
	{
		GLightComponent*& primitive = lightComponents[i];

		bool bUpdated = false;
		if (primitive->HasAnyDirtyMark())
		{
			primitive->ResolveDirtyState();
			bUpdated = true;
		}

		if (bUpdated || bLightCountUpdated)
		{
			lightProxies[i] = primitive->GetSceneProxy();
			if (lightProxies[i] != nullptr)
			{
				lightProxies[i]->PrimitiveId = i;
				lightInstanced->Emplace<HomogeneousLight>(i, lightProxies[i]->PrimitiveLight);
			}
			else
			{
				lightInstanced->Emplace<HomogeneousLight>(i, nullLight);
			}
		}
	}

	if (bInstancingBufferUpdated)
	{
		UpdateInstancingBufferSRV();
	}

	if (bLightCountUpdated)
	{
		UpdateLightInstancedSRV();
	}
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
	shaderManager->ResolveMaterials();
	instancingScene->BuildScene(deviceContext);
	allocator->BeginAllocate((uint32)numSRVs);
}

void Scene::EndRender(DirectXDeviceContext* deviceContext)
{
	allocator->EndAllocate();
}

void Scene::AddPrimitive(GPrimitiveComponent* inPrimitiveComponent)
{
	const size_t index = primitiveComponents.size();
	PrimitiveSceneProxy* sceneProxy = inPrimitiveComponent->GetSceneProxy();

	primitiveComponents.emplace_back(inPrimitiveComponent);
	sceneProxies.emplace_back(sceneProxy);

	if (sceneProxy != nullptr)
	{
		sceneProxy->PrimitiveId = index;
		instancingScene->AddInstance(index, sceneProxy);

		UpdateInstancingBufferSRV();
	}
}

void Scene::AddLight(GLightComponent* inLightComponent)
{
	const size_t index = lightComponents.size();
	LightSceneProxy* sceneProxy = inLightComponent->GetSceneProxy();

	lightComponents.emplace_back(inLightComponent);
	lightProxies.emplace_back(sceneProxy);

	if (sceneProxy != nullptr)
	{
		sceneProxy->PrimitiveId = index;
		lightInstanced->Count = max(lightInstanced->Count, index + 1);
		lightInstanced->Emplace<HomogeneousLight>(index, sceneProxy->PrimitiveLight);

		UpdateLightInstancedSRV();
	}
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

void Scene::UpdateInstancingBufferSRV()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = { };
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.RaytracingAccelerationStructure.Location = instancingScene->GetRaytracingAccelerationStructureBuffer();

	sceneSRV->CreateShaderResourceView(0, nullptr, &srvDesc);
}

void Scene::UpdateLightInstancedSRV()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = { };
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Buffer.NumElements = (uint32)lightInstanced->Count;
	srvDesc.Buffer.StructureByteStride = sizeof(HomogeneousLight);

	sceneSRV->CreateShaderResourceView(1, lightInstanced->Resource, &srvDesc);
}