// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "World.h"
#include "Actors/Actor.h"
#include "Scene/SceneView.h"

AWorld::AWorld()
{
}

AWorld::~AWorld() noexcept
{
}

void AWorld::InitializeScene()
{
	SceneView = std::make_shared<NSceneView>();
}

void AWorld::DestroyScene()
{
	SceneView.reset();
}

AActor* AWorld::SpawnPersistentActor(AType* InActorType)
{
	RefPtr<AActor>& Actor = PersistentActors.emplace_back(NewObject<AActor>(InActorType));
	return Actor.Get();
}