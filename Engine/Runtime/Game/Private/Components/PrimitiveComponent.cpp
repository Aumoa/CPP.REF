// Copyright 2020 Aumoa.lib. All right reserved.

#include "Components/PrimitiveComponent.h"

#include "SceneRendering/PrimitiveSceneProxy.h"

PrimitiveComponent::PrimitiveComponent() : Super()
{

}

PrimitiveComponent::~PrimitiveComponent()
{

}

void PrimitiveComponent::ResolveDirtyState()
{
	if (HasDirtyMark(EComponentDirtyMask::RecreateProxy))
	{
		sceneProxy = CreateSceneProxy();
	}

	if (HasDirtyMark(EComponentDirtyMask::TransformUpdated) && sceneProxy.IsValid)
	{
		sceneProxy->UpdateMovable();
	}

	Super::ResolveDirtyState();
}

TRefPtr<PrimitiveSceneProxy> PrimitiveComponent::CreateSceneProxy()
{
	return nullptr;
}

PrimitiveSceneProxy* PrimitiveComponent::GetSceneProxy() const
{
	return sceneProxy.Get();
}