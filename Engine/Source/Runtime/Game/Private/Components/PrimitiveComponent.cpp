// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Components/PrimitiveComponent.h"
#include "Level/World.h"
#include "Scene/PrimitiveSceneProxy.h"

SPrimitiveComponent::SPrimitiveComponent() : Super()
{
}

SPrimitiveComponent::~SPrimitiveComponent()
{
	if (!ensure(SceneProxy == nullptr))
	{
		delete SceneProxy;
		SceneProxy = nullptr;
	}
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
}