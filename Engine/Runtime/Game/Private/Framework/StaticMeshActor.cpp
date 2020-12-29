// Copyright 2020 Aumoa.lib. All right reserved.

#include "Framework/StaticMeshActor.h"

#include "Components/StaticMeshComponent.h"

AStaticMeshActor::AStaticMeshActor() : Super()
	, staticMeshComponent(nullptr)
{
	auto component = NewObject<StaticMeshComponent>();
	RootComponent = component.Get();
	staticMeshComponent = component.Get();
}

AStaticMeshActor::~AStaticMeshActor()
{

}