// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "World.h"
#include "Actors/Actor.h"

AWorld::AWorld()
{
}

AWorld::~AWorld() noexcept
{
}

AActor* AWorld::SpawnPersistentActor(AType* InActorType)
{
	RefPtr<AActor>& Actor = PersistentActors.emplace_back(NewObject<AActor>(InActorType));
	return Actor.Get();
}

void AWorld::BeginPlay()
{
}

void AWorld::EndPlay()
{
}