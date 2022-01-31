// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Scene/PrimitiveSceneProxy.h"
#include "Components/PrimitiveComponent.h"
#include "RenderThread.h"

PrimitiveSceneProxy::PrimitiveSceneProxy(SPrimitiveComponent* InPrimitiveComponent) : PrimitiveComponent(InPrimitiveComponent)
	, PrimitiveId(-1)
	, ComponentTransform(InPrimitiveComponent->GetComponentTransform())
	, bRenderStateDirty(true)
	, bHiddenInGame(InPrimitiveComponent->IsHiddenInGame())
{
}

Task<void> PrimitiveSceneProxy::UpdateTransform_GameThread(Transform InValue)
{
	co_await RenderThread::EnqueueRenderThreadAwaiter();
	UpdateTransform_RenderThread(InValue);
}

void PrimitiveSceneProxy::UpdateTransform_RenderThread(const Transform& InValue)
{
	checkf(RenderThread::IsInRenderThread(), L"Callstack is not in render thread.");
	ComponentTransform = InValue;
}

Task<void> PrimitiveSceneProxy::MarkRenderStateDirty_GameThread()
{
	co_await RenderThread::EnqueueRenderThreadAwaiter();
	MarkRenderStateDirty_RenderThread();
}

void PrimitiveSceneProxy::MarkRenderStateDirty_RenderThread()
{
	checkf(RenderThread::IsInRenderThread(), L"Callstack is not in render thread.");
	bRenderStateDirty = true;
}

Task<void> PrimitiveSceneProxy::SetHiddenInGame_GameThread(bool bHiddenInGame)
{
	co_await RenderThread::EnqueueRenderThreadAwaiter();
	SetHiddenInGame_RenderThread(bHiddenInGame);
}

void PrimitiveSceneProxy::SetHiddenInGame_RenderThread(bool bHiddenInGame)
{
	checkf(RenderThread::IsInRenderThread(), L"Callstack is not in render thread.");
	this->bHiddenInGame = bHiddenInGame;
}