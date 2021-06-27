// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "PrimitiveSceneProxy.h"
#include "Components/PrimitiveComponent.h"

PrimitiveSceneProxy::PrimitiveSceneProxy(PrimitiveComponent* inComponent) : Super()
	, _MyComponent(inComponent)
{
	_bHiddenInGame = inComponent->IsHiddenInGame();
}

void PrimitiveSceneProxy::UpdateTransform_GameThread(const Transform& value)
{
	ComponentTransform = value;
}

void PrimitiveSceneProxy::MarkRenderStateDirty_GameThread()
{
	bRenderStateDirty = true;
}

void PrimitiveSceneProxy::SetHiddenInGame_GameThread(bool bHiddenInGame)
{
	_bHiddenInGame = bHiddenInGame;
}