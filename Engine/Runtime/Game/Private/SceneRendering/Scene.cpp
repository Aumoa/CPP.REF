// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/Scene.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::Render(SceneRenderer* renderer)
{
	
}

void Scene::AddScene(PrimitiveSceneProxy* inSceneProxy)
{
	sceneProxies.emplace_back(inSceneProxy);
}