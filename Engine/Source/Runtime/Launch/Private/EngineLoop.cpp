// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "EngineLoop.h"
#include "Launch.h"
#include "GenericPlatform/GenericApplication.h"
#include "GenericPlatform/GenericSplash.h"
#include "Bootstrap/BootstrapTask.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"
#include "Application/SlateApplication.h"
#include "SlateRHIRenderer.h"
#include "Widgets/SWindow.h"
#include "GC.h"
#include "SlateRHIRenderer.h"

NEngineLoop::NEngineLoop()
{
}

NEngineLoop::~NEngineLoop() noexcept
{
}

void NEngineLoop::Init(NInitializeContext* InContext)
{
    InitScripts(InContext);
    InitRHIs(InContext);
}

void NEngineLoop::Tick()
{
    NSlateApplication::Get().Tick();
    NSlateApplication::Get().DispatchQueuedRenderingWorks();
}

void NEngineLoop::InitScripts(NInitializeContext* InContext)
{
    if (InContext) InContext->ScriptsTask->Step(0.0f);
    NGC::Init();
    if (InContext) InContext->ScriptsTask->Step(100.0f);
}

void NEngineLoop::InitRHIs(NInitializeContext* InContext)
{
    if (InContext) InContext->GraphicsTask->Step(0.0f);
    NRHIGlobal::InitDynamicRHI();
    if (InContext) InContext->GraphicsTask->Step(90.0f);
    auto Renderer = std::make_shared<NSlateRHIRenderer>();
    NSlateApplication::Create().SetupSlateRenderer(std::move(Renderer));
    if (InContext) InContext->GraphicsTask->Step(100.0f);
}