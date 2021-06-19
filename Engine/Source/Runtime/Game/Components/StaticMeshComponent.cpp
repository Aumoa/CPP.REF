// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "StaticMeshComponent.h"
#include "Scene/StaticMeshSceneProxy.h"

StaticMeshComponent::StaticMeshComponent() : Super()
{
}

PrimitiveSceneProxy* StaticMeshComponent::CreateSceneProxy()
{
	return new StaticMeshSceneProxy(this);
}

void StaticMeshComponent::SetStaticMesh(StaticMesh* inNewMesh)
{
	_StaticMesh = inNewMesh;
}