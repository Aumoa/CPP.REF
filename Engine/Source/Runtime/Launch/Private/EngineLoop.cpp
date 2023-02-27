// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "EngineLoop.h"
#include "GenericPlatform/GenericApplication.h"

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

    // Create initialization context.
    auto Context = std::make_unique<NInitializeContext>();

    Vector2N Resolution = GenericApp->GetScreenResolution();
    Vector2N CenterPos = Resolution / 2;

    NGenericWindowDefinition SplashDef;
    SplashDef.bPopup = true;
    SplashDef.DesiredScreenSize = Vector2N(600, 200);
    SplashDef.DesiredScreenPosition = CenterPos - (SplashDef.DesiredScreenSize / 2);

    Context->SplashWindow = GenericApp->MakeWindow(SplashDef);

    // Show splash window and finish pre-initialize.
    Context->SplashWindow->Show();
    return Context;
}

int32 NEngineLoop::GetExitCode() const
{
    return 0;
}