// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "EngineLoop.h"
#include "Launch.h"
#include "GenericPlatform/GenericApplication.h"
#include "GenericPlatform/GenericSplash.h"

constexpr LogCategory LogEngineLoop(TEXT("LogEngineLoop"));

NEngineLoop::NEngineLoop()
{
}

NEngineLoop::~NEngineLoop() noexcept
{
}

std::unique_ptr<NEngineLoop::NInitializeContext> NEngineLoop::PreInit()
{
    Log::Info(LogEngineLoop, TEXT("Start pre-initialize engine."));
    GenericApp = NGenericApplication::CreateApplication();
    GenericApp->SetApplicationPointer(NLaunch::Get().GetApplicationPointer());

    // Show splash window.
    NGenericSplash::Show();

    // Create initialization context.
    auto Context = std::make_unique<NInitializeContext>();
    return Context;
}

int32 NEngineLoop::GetExitCode() const
{
    return 0;
}