// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Components/StaticMeshComponent.h"

#include "SceneRendering/StaticMeshSceneProxy.h"
#include "SceneRendering/StaticMesh.h"
#include "Materials/MaterialInterface.h"

StaticMeshComponent::StaticMeshComponent()
{

}

StaticMeshComponent::~StaticMeshComponent()
{

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

void StaticMeshComponent::SetStaticMesh(StaticMesh* inMesh)
{
	staticMesh = inMesh;
	overrideMaterial = staticMesh->DefaultMaterial;
	SetMarkDirty(EComponentDirtyMask::RecreateProxy);
}

StaticMesh* StaticMeshComponent::GetStaticMesh() const
{
	return staticMesh.Get();
}

void StaticMeshComponent::SetMaterial(MaterialInterface* inMaterial)
{
	overrideMaterial = inMaterial;
	SetMarkDirty(EComponentDirtyMask::UpdateProxy);
}

MaterialInterface* StaticMeshComponent::GetMaterial() const
{
	return overrideMaterial.Get();
}