// Copyright 2020-2021 Aumoa.lib. All right reserved.

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
	bool bRecreated = false;
	if (HasDirtyMark(EComponentDirtyMask::RecreateProxy))
	{
		sceneProxy = CreateSceneProxy();
		bRecreated = true;
	}

	if (sceneProxy.IsValid)
	{
		if (bRecreated || HasDirtyMark(EComponentDirtyMask::UpdateProxy))
		{
			sceneProxy->Update();
		}

		if (bRecreated || HasDirtyMark(EComponentDirtyMask::TransformUpdated))
		{
			sceneProxy->UpdateTransform();
		}
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