// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "EngineLoop.h"
#include "Launch.h"
#include "GenericPlatform/GenericApplication.h"
#include "GenericPlatform/GenericSplash.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"
#include "Application/SlateApplication.h"
#include "SlateRHIRenderer.h"
#include "Widgets/SWindow.h"
#include "GC.h"
#include "SlateRHIRenderer.h"
#include "Widgets/SWindow.h"

NEngineLoop::NEngineLoop()
{
}

NEngineLoop::~NEngineLoop() noexcept
{
}

void NEngineLoop::Init()
{
}

void NEngineLoop::Tick()
{
    NSlateApplication& SlateApp = NSlateApplication::Get();
    SlateApp.Tick();
    SlateApp.DispatchQueuedRenderingWorks();
}

void NEngineLoop::PreInit(String CmdArgs)
{
    PreInitPreStartupScreen(CmdArgs);

    // Show splash window.
    NGenericSplash::Show();

    PreInitPostStartupScreen();
}

void NEngineLoop::PostInit()
{
    NGenericSplash::Hide();
    NSlateApplication::Get().SetupCoreWindow(SNew(SWindow).Visibility(ESlateVisibility::Visible));
}

void NEngineLoop::PreInitPreStartupScreen(String CmdArgs)
{
    // Setup command line arguments.
    CommandLine::Init(CmdArgs);

    // Initialize GC engine.
    NGC::Init();

    // Initialize RHI engine.
    NRHIGlobal::InitDynamicRHI();

    // Initialize slate application.
    auto Renderer = std::make_shared<NSlateRHIRenderer>();
    NSlateApplication::Create().SetupSlateRenderer(std::move(Renderer));
}

void NEngineLoop::PreInitPostStartupScreen()
{
}