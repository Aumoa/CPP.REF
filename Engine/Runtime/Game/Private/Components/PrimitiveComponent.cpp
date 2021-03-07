// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Components/PrimitiveComponent.h"

#include "SceneRendering/PrimitiveSceneProxy.h"

GPrimitiveComponent::GPrimitiveComponent() : Super()
{

}

GPrimitiveComponent::~GPrimitiveComponent()
{

}

void GPrimitiveComponent::ResolveDirtyState()
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

TRefPtr<PrimitiveSceneProxy> GPrimitiveComponent::CreateSceneProxy()
{
	return nullptr;
}

PrimitiveSceneProxy* GPrimitiveComponent::GetSceneProxy() const
{
	return sceneProxy.Get();
}