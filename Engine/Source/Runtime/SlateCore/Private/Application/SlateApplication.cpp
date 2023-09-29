// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Application/SlateApplication.h"
#include "Widgets/SWindow.h"
#include "Rendering/SlateRenderer.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"

std::shared_ptr<NSlateApplication> NSlateApplication::SlateApp;

NSlateApplication::NSlateApplication()
{
}

NSlateApplication::~NSlateApplication() noexcept
{
}

void NSlateApplication::Tick()
{
    TimeSpan DeltaTime = Timer.Tick();
    NSlateWindowElementList DrawElements;
    CoreWindow->ExecuteTick(DeltaTime);

    NRHIGraphics& Graphics = NRHIGlobal::GetDynamicRHI();
    Graphics.BeginFrame();

    Renderer->BeginFrame();
    CoreWindow->Render(DeltaTime, *Renderer);
    Renderer->EndFrame();

    PresentAllWindows();
    Graphics.EndFrame();
}

void NSlateApplication::PresentAllWindows()
{
    CoreWindow->Present();
}

void NSlateApplication::SetupCoreWindow(std::shared_ptr<SWindow> InCoreWindow)
{
    CoreWindow = std::move(InCoreWindow);
    CreatePlatformWindow(*CoreWindow);
}

void NSlateApplication::SetupSlateRenderer(std::shared_ptr<NSlateRenderer> InRenderer)
{
    Renderer = std::move(InRenderer);
}

void NSlateApplication::CreatePlatformWindow(SWindow& InSlateWindow)
{
    check(Renderer);
    Renderer->CreateViewport(InSlateWindow);
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