// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "EngineLoop.h"
#include "Launch.h"
#include "GenericPlatform/GenericApplication.h"
#include "GenericPlatform/GenericSplash.h"
#include "Bootstrap/BootstrapTask.h"
#include "RHI/RHIGraphics.h"
#include "RHI/RHICommandQueue.h"
#include "Application/SlateApplication.h"
#include "Widgets/SWindow.h"

NEngineLoop::NEngineLoop()
{
}

NEngineLoop::~NEngineLoop() noexcept
{
}

void NEngineLoop::Init(NInitializeContext* InContext)
{
    if (InContext) InContext->GraphicsTask->Step(0.0f);
    auto RHI = NRHIGraphics::GenerateGraphics(TEXT("D3D12"));
    RHI->Init();

    if (InContext) InContext->GraphicsTask->Step(90.0f);
    auto CommandQueue = RHI->CreateCommandQueue();

    SlateApp = std::make_unique<NSlateApplication>(NGenericApplication::Get());

    if (InContext) InContext->GraphicsTask->Step(100.0f);
}

void NEngineLoop::Tick()
{
}