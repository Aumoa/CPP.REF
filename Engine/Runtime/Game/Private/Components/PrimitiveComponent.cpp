// Copyright 2020 Aumoa.lib. All right reserved.

#include "Components/PrimitiveComponent.h"

#include "SceneRendering/PrimitiveSceneProxy.h"

PrimitiveComponent::PrimitiveComponent() : Super()
{

}

PrimitiveComponent::~PrimitiveComponent()
{

}

TRefPtr<PrimitiveSceneProxy> PrimitiveComponent::CreateSceneProxy()
{
	return nullptr;
}

PrimitiveSceneProxy* PrimitiveComponent::GetSceneProxy() const
{
	return sceneProxy.Get();
}

void PrimitiveComponent::SetDirtyMark()
{
	bHasDirtyMark = true;
}

bool PrimitiveComponent::HasDirtyMark() const
{
	return bHasDirtyMark;
}

void PrimitiveComponent::ResolveDirtyState()
{
	sceneProxy = CreateSceneProxy();
	bHasDirtyMark = false;
}