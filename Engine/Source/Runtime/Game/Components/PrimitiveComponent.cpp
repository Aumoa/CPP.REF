// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "PrimitiveComponent.h"
#include "Level/World.h"
#include "Scene/PrimitiveSceneProxy.h"

PrimitiveComponent::PrimitiveComponent() : Super()
{
}

void PrimitiveComponent::SetHiddenInGame(bool bHidden)
{
	_bHiddenInGame = bHidden;

	if (SceneProxy != nullptr)
	{
		SceneProxy->SetHiddenInGame_GameThread(bHidden);
	}
}

void PrimitiveComponent::MarkRenderStateDirty()
{
	if (SceneProxy == nullptr)
	{
		if (World* world = GetWorld(); world != nullptr)
		{
			// Re-register primitive component for add scene proxy to actual scene.
			world->RegisterComponent(this);
		}
	}
	else
	{
		SceneProxy->MarkRenderStateDirty_GameThread();
	}
}