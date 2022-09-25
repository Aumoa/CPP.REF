// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Actors/SampleActor.h"
#include "Components/StaticMeshComponent.h"
#include "SampleActor.gen.cpp"

ASampleActor::ASampleActor()
{
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(NewObject<StaticMeshComponent>());
}

void ASampleActor::Tick(const TimeSpan& InDeltaTime)
{
	Super::Tick(InDeltaTime);
}