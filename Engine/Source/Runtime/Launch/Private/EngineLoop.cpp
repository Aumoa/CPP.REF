// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "EngineLoop.h"
#include "Launch.h"
#include "GameEngine.h"
#include "RenderGlobal.h"
#include "GenericPlatform/GenericApplication.h"
#include "GenericPlatform/GenericSplash.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"
#include "Application/SlateApplication.h"
#include "SlateRHIRenderer.h"
#include "Widgets/SWindow.h"
#include "SlateRHIRenderer.h"
#include "SlateGlobalShaders.h"
#include "Rendering/SlateTextFormatCache.h"
#include "Widgets/SWindow.h"
#include "Widgets/SViewport.h"

NEngineLoop::NEngineLoop()
{
}

NEngineLoop::~NEngineLoop() noexcept
{
}

void NEngineLoop::Init(TSubclassOf<AGameEngine> InEngineClass)
{
    EngineInstance = NewObject(InEngineClass);
    EngineInstance->Initialize();
}

void NEngineLoop::Shutdown()
{
    // shutdown engine instance.
    EngineInstance->Deinitialize();
    EngineInstance.Reset();

    // waiting for all graphics commands are completed.
    NRHIGlobal::GetDynamicRHI().SyncFrame();

    // shutdown application first.
    NSlateApplication::Destroy();

    // shutdown graphics engine.
    NSlateTextFormatCache::Shutdown();
    NSlateGlobalShaders::Shutdown();
    NRenderGlobal::Shutdown();
    NRHIGlobal::ShutdownDynamicRHI();
}

void NEngineLoop::Tick(const std::vector<NGenericPlatformInputEvent>& InputEvents)
{
    NSlateApplication& SlateApp = NSlateApplication::Get();
    SlateApp.Tick(InputEvents);
}

void NEngineLoop::PreInit(String CmdArgs)
{
    PreInitPreStartupScreen(CmdArgs);

#if WITH_EDITOR
    // Show splash window. 
    NGenericSplash::Show();
#endif

    PreInitPostStartupScreen();
}

void NEngineLoop::PostInit()
{
    NGenericSplash::Hide();
    NSlateApplication::Get().GetCoreWindow().SetVisibility(ESlateVisibility::Visible);
}

void NEngineLoop::PreInitPreStartupScreen(String CmdArgs)
{
    // Setup command line arguments.
    CommandLine::Init(CmdArgs);

    // Initialize RHI engine.
    NRHIGlobal::InitDynamicRHI();
    NRenderGlobal::Initialize();
    NSlateGlobalShaders::Initialize();
    NSlateTextFormatCache::Initialize();

    // Initialize slate application.
    auto Renderer = std::make_shared<NSlateRHIRenderer>();
    NSlateApplication::Create().SetupSlateRenderer(std::move(Renderer));
}

void NEngineLoop::PreInitPostStartupScreen()
{
    std::shared_ptr<SWindow> CoreWindow;

    NSlateApplication::Get().SetupCoreWindow(
        SAssignNew(CoreWindow, SWindow)
        .Visibility(ESlateVisibility::Hidden)
    );

    CoreWindow->SetContent(SAssignNew(GameViewport, SViewport).bAllocateViewport(true));
}