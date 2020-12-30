// Copyright 2020 Aumoa.lib. All right reserved.

#include "Components/StaticMeshComponent.h"

#include "SceneRendering/StaticMeshSceneProxy.h"

StaticMeshComponent::StaticMeshComponent()
{

}

StaticMeshComponent::~StaticMeshComponent()
{

}

TRefPtr<PrimitiveSceneProxy> StaticMeshComponent::CreateSceneProxy()
{
	return NewObject<StaticMeshSceneProxy>(this);
}