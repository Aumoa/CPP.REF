// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Scene/PrimitiveSceneProxy.h"
#include "Components/PrimitiveComponent.h"
#include "GameThreads/RenderThread.h"

#define DEFINE_REDIRECT_RENDER_THREAD(FunctionName) \
void PrimitiveSceneProxy::FunctionName ## _GameThread() \
{ \
	RenderThread::EnqueueRenderThreadWork<"PrimitiveSceneProxy::" #FunctionName>([this](auto) { FunctionName ## _RenderThread(); }); \
}

#define DEFINE_REDIRECT_RENDER_THREAD_OneParam(FunctionName, Type1) \
void PrimitiveSceneProxy::FunctionName ## _GameThread(Type1 Param1) \
{ \
	RenderThread::EnqueueRenderThreadWork<"PrimitiveSceneProxy::" #FunctionName>([this, Param1 = Param1](auto) { FunctionName ## _RenderThread(Param1); }); \
}

PrimitiveSceneProxy::PrimitiveSceneProxy(SPrimitiveComponent* InPrimitiveComponent) : PrimitiveComponent(InPrimitiveComponent)
	, PrimitiveId(-1)
	, ComponentTransform(InPrimitiveComponent->GetComponentTransform())
	, bRenderStateDirty(true)
	, bHiddenInGame(InPrimitiveComponent->IsHiddenInGame())
{
}

DEFINE_REDIRECT_RENDER_THREAD_OneParam(UpdateTransform, const Transform&);

void PrimitiveSceneProxy::UpdateTransform_RenderThread(const Transform& InValue)
{
	check(RenderThread::IsInRenderThread());
	ComponentTransform = InValue;
}

DEFINE_REDIRECT_RENDER_THREAD(MarkRenderStateDirty);

void PrimitiveSceneProxy::MarkRenderStateDirty_RenderThread()
{
	check(RenderThread::IsInRenderThread());
	bRenderStateDirty = true;
}

DEFINE_REDIRECT_RENDER_THREAD_OneParam(SetHiddenInGame, bool);

void PrimitiveSceneProxy::SetHiddenInGame_RenderThread(bool bHiddenInGame)
{
	check(RenderThread::IsInRenderThread());
	this->bHiddenInGame = bHiddenInGame;
}

#undef DEFINE_REDIRECT_RENDER_THREAD
#undef DEFINE_REDIRECT_RENDER_THREAD_OneParam