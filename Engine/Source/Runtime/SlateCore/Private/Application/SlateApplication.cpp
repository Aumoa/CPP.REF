// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Application/SlateApplication.h"
#include "Widgets/SWindow.h"
#include "GenericPlatform/GenericWindow.h"

std::shared_ptr<NSlateApplication> NSlateApplication::SlateApp;

NSlateApplication::NSlateApplication()
{
}

NSlateApplication::~NSlateApplication() noexcept
{
}

void NSlateApplication::Tick()
{
}

void NSlateApplication::SetupCoreWindow(std::shared_ptr<SWindow> InCoreWindow)
{
    CoreWindow = std::move(InCoreWindow);
    CreatePlatformWindow(CoreWindow.get());
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

std::shared_ptr<NSlateApplication> NSlateApplication::Create()
{
    auto Ptr = std::shared_ptr<NSlateApplication>(new NSlateApplication());
    SlateApp = Ptr;
    return Ptr;
}

void NSlateApplication::Destroy()
{
    SlateApp.reset();
}

NSlateApplication& NSlateApplication::Get()
{
    return *SlateApp;
}