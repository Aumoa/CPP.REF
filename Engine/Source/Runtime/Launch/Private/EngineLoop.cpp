// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "EngineLoop.h"
#include "Launch.h"
#include "GenericPlatform/GenericApplication.h"
#include "GenericPlatform/GenericSplash.h"
#include "Bootstrap/BootstrapTask.h"
#include "RHI/RHIGraphics.h"

constexpr LogCategory LogEngineLoop(TEXT("LogEngineLoop"));

NEngineLoop::NEngineLoop()
{
}

NEngineLoop::~NEngineLoop() noexcept
{
}

NEngineLoop::NInitializeContext* NEngineLoop::PreInit()
{
    Log::Info(LogEngineLoop, TEXT("Start pre-initialize engine."));
    GenericApp = NGenericApplication::CreateApplication();
    GenericApp->SetApplicationPointer(NLaunch::Get().GetApplicationPointer());

    // Show splash window.
    NGenericSplash::Show();

    // Create initialization context.
    Context = std::make_unique<NInitializeContext>();
    Context->GraphicsTask.emplace(TEXT("Graphics"), 100.0f);
    return Context.get();
}

int32 NEngineLoop::GetExitCode() const
{
    return 0;
}

void NEngineLoop::Init()
{
    Context->GraphicsTask->Step(0.0f);
    auto RHI = NRHIGraphics::GenerateGraphics(TEXT("D3D12"));
    RHI->Init();
    Context->GraphicsTask->Step(100.0f);
}