// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Scene/PrimitiveSceneProxy.h"
#include "Components/PrimitiveComponent.h"

SPrimitiveSceneProxy::SPrimitiveSceneProxy(SPrimitiveComponent* inComponent) : Super()
	, _MyComponent(inComponent)
{
	_bHiddenInGame = inComponent->IsHiddenInGame();
}

void SPrimitiveSceneProxy::UpdateTransform_GameThread(const Transform& value)
{
	ComponentTransform = value;
}

void SPrimitiveSceneProxy::MarkRenderStateDirty_GameThread()
{
	bRenderStateDirty = true;
}

void SPrimitiveSceneProxy::SetHiddenInGame_GameThread(bool bHiddenInGame)
{
	_bHiddenInGame = bHiddenInGame;
}