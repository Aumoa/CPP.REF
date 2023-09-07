// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Application/SlateApplication.h"
#include "Widgets/SWindow.h"
#include "GenericPlatform/GenericWindow.h"
#include "Rendering/SlateRenderer.h"

std::shared_ptr<NSlateApplication> NSlateApplication::SlateApp;

NSlateApplication::NSlateApplication()
{
}

NSlateApplication::~NSlateApplication() noexcept
{
}

void NSlateApplication::Tick()
{
    Renderer->BeginFrame();
    Renderer->EndFrame();
}

void NSlateApplication::PresentAllWindows()
{
    CoreWindow->Present();
}

void NSlateApplication::SetupCoreWindow(std::shared_ptr<SWindow> InCoreWindow)
{
    CoreWindow = std::move(InCoreWindow);
    CreatePlatformWindow(CoreWindow.get());
}

void NSlateApplication::SetupSlateRenderer(std::shared_ptr<NSlateRenderer> InRenderer)
{
    Renderer = std::move(InRenderer);
}

void NSlateApplication::CreatePlatformWindow(SWindow* InSlateWindow)
{
    NGenericWindowDefinition Def;
    Def.bPrimaryWindow = true;
    Def.bSystemMenu = true;
    Def.bThickframe = true;
    Def.bSizebox = true;
    Def.bCaption = true;
    std::shared_ptr GenericWindow = NGenericApplication::Get().MakeWindow(Def);
    InSlateWindow->AttachWindow(std::move(GenericWindow));
}

NSlateApplication& NSlateApplication::Create()
{
    SlateApp = std::shared_ptr<NSlateApplication>(new NSlateApplication());
    return Get();
}

void NSlateApplication::Destroy()
{
    SlateApp.reset();
}

NSlateApplication& NSlateApplication::Get()
{
    return *SlateApp;
}