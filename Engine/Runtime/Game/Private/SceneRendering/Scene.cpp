// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/Scene.h"

#include "Components/PrimitiveComponent.h"
#include "SceneRendering/SceneRenderer.h"

Scene::Scene()
{

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
		}
	}
}

void Scene::Render(SceneRenderer* renderer)
{
	renderer->AddPrimitives(sceneProxies);
}

void Scene::AddScene(PrimitiveComponent* inPrimitiveComponent)
{
	primitiveComponents.emplace_back(inPrimitiveComponent);
	sceneProxies.emplace_back(inPrimitiveComponent->GetSceneProxy());
}