// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Components/PrimitiveComponent.h"
#include "Level/World.h"
#include "Scene/PrimitiveSceneProxy.h"

SPrimitiveComponent::SPrimitiveComponent() : Super()
{
}

void SPrimitiveComponent::SetHiddenInGame(bool bHidden)
{
	if (_bHiddenInGame != bHidden)
	{
		_bHiddenInGame = bHidden;

		if (SceneProxy != nullptr)
		{
			SceneProxy->SetHiddenInGame_GameThread(bHidden);
		}
	}
}

void SPrimitiveComponent::MarkRenderStateDirty()
{
	if (SceneProxy == nullptr)
	{
		if (SWorld* world = GetWorld(); world != nullptr)
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