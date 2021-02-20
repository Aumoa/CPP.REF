// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Framework/StaticMeshActor.h"

#include "Components/StaticMeshComponent.h"

AStaticMeshActor::AStaticMeshActor() : Super()
	, staticMeshComponent(nullptr)
{
	auto component = AddComponent<StaticMeshComponent>();
	RootComponent = component;
	staticMeshComponent = component;
}

AStaticMeshActor::~AStaticMeshActor()
{

}

StaticMeshComponent* AStaticMeshActor::StaticMesh_get() const
{
	return staticMeshComponent;
}