// Copyright 2020 Aumoa.lib. All right reserved.

#include "Components/StaticMeshComponent.h"

#include "SceneRendering/StaticMeshSceneProxy.h"
#include "SceneRendering/StaticMesh.h"

StaticMeshComponent::StaticMeshComponent()
{

}

StaticMeshComponent::~StaticMeshComponent()
{

}

void StaticMeshComponent::SetStaticMesh(StaticMesh* inMesh)
{
	staticMesh = inMesh;
	SetMarkDirty(EComponentDirtyMask::RecreateProxy);
}

StaticMesh* StaticMeshComponent::GetStaticMesh() const
{
	return staticMesh.Get();
}

TRefPtr<PrimitiveSceneProxy> StaticMeshComponent::CreateSceneProxy()
{
	if (staticMesh.IsValid)
	{
		return NewObject<StaticMeshSceneProxy>(this);
	}
	else
	{
		return nullptr;
	}
}