// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GameplayStatics/ClassDependencyHelper.h"

#include "World.h"
#include "GameObject.h"
#include "SceneRendering/Scene.h"
#include "Logging/LogMacros.h"

Engine* ClassDependencyHelper::GetEngine(GGameObject* inWorldContext)
{
	if (inWorldContext == nullptr)
	{
		SE_LOG(LogEngine, Error, L"inWorldContext is nullptr.");
		return nullptr;
	}

	World* world = inWorldContext->GetWorld();
	if (world == nullptr)
	{
		SE_LOG(LogEngine, Error, L"inWorldContext is not contains to world.");
		return nullptr;
	}

	return GetEngine(world);
}

Engine* ClassDependencyHelper::GetEngine(World* inWorld)
{
	Scene* scene = inWorld->GetScene();
	return scene->GetEngine();
}