// Copyright 2020-2022 Aumoa.lib. All right reserved.

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

void PrimitiveSceneProxy::UpdateTransform_GameThread(Transform InValue)
{
	RenderThread::Get()->EnqueueRenderThreadWork(nullptr, [=](auto)
	{
		UpdateTransform_RenderThread(InValue);
	});
}

void PrimitiveSceneProxy::UpdateTransform_RenderThread(const Transform& InValue)
{
	checkf(RenderThread::InRenderThread(), L"Callstack is not in render thread.");
	ComponentTransform = InValue;
}

void PrimitiveSceneProxy::MarkRenderStateDirty_GameThread()
{
	RenderThread::Get()->EnqueueRenderThreadWork(nullptr, [=](auto)
	{
		MarkRenderStateDirty_RenderThread();
	});
}

void PrimitiveSceneProxy::MarkRenderStateDirty_RenderThread()
{
	checkf(RenderThread::InRenderThread(), L"Callstack is not in render thread.");
	bRenderStateDirty = true;
}

void PrimitiveSceneProxy::SetHiddenInGame_GameThread(bool bHiddenInGame)
{
	RenderThread::Get()->EnqueueRenderThreadWork(nullptr, [=](auto)
	{
		SetHiddenInGame_RenderThread(bHiddenInGame);
	});
}

void PrimitiveSceneProxy::SetHiddenInGame_RenderThread(bool bHiddenInGame)
{
	checkf(RenderThread::InRenderThread(), L"Callstack is not in render thread.");
	this->bHiddenInGame = bHiddenInGame;
}