// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/Scene.h"

#include "Components/PrimitiveComponent.h"
#include "Framework/PlayerController.h"
#include "Components/PlayerCameraManager.h"
#include "Shaders/ShaderCameraConstant.h"
#include "SceneRendering/PrimitiveSceneProxy.h"

using namespace std;

Scene::Scene() : Super()
	, localPlayer(nullptr)
{
	shaderCamConstants = NewObject<ShaderCameraConstantVector>();
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
}

void Scene::AddScene(PrimitiveComponent* inPrimitiveComponent)
{
	primitiveComponents.emplace_back(inPrimitiveComponent);
	sceneProxies.emplace_back(inPrimitiveComponent->GetSceneProxy());
}

APlayerController* Scene::LocalPlayer_get() const
{
	return localPlayer;
}

void Scene::LocalPlayer_set(APlayerController* value)
{
	localPlayer = value;
}

span<PrimitiveComponent* const> Scene::Primitives_get() const
{
	return primitiveComponents;
}

span<PrimitiveSceneProxy* const> Scene::PrimitiveSceneProxies_get() const
{
	return sceneProxies;
}

ShaderCameraConstantVector* Scene::ShaderCameraConstants_get() const
{
	return shaderCamConstants.Get();
}