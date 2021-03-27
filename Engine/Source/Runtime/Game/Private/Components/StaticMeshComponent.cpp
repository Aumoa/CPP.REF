// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Components/StaticMeshComponent.h"

#include "SceneRendering/StaticMeshSceneProxy.h"
#include "SceneRendering/StaticMesh.h"
#include "Materials/MaterialInterface.h"

GStaticMeshComponent::GStaticMeshComponent()
{

}

GStaticMeshComponent::~GStaticMeshComponent()
{

}

TRefPtr<PrimitiveSceneProxy> GStaticMeshComponent::CreateSceneProxy()
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

void GStaticMeshComponent::SetStaticMesh(StaticMesh* inMesh)
{
	staticMesh = inMesh;
	SetMarkDirty(EComponentDirtyMask::RecreateProxy);
}

StaticMesh* GStaticMeshComponent::GetStaticMesh() const
{
	return staticMesh.Get();
}