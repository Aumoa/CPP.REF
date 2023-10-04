// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericApplication.h"

class SWindow;
class NSlateRenderer;
class SViewport;

class SLATECORE_API NSlateApplication
{
    NSlateApplication(const NSlateApplication&) = delete;
    NSlateApplication(NSlateApplication&&) = delete;
    static std::shared_ptr<NSlateApplication> SlateApp;

private:
    std::shared_ptr<SWindow> CoreWindow;
    std::shared_ptr<NSlateRenderer> Renderer;
    TickTimer Timer;
    std::shared_ptr<SViewport> GameViewport;

private:
    NSlateApplication();

public:
    virtual ~NSlateApplication() noexcept;

    void Tick();
    void PresentAllWindows();

    void SetupCoreWindow(std::shared_ptr<SWindow> InCoreWindow);
    SWindow& GetCoreWindow() { return *CoreWindow; }

    void SetupGameViewport(std::shared_ptr<SViewport> InGameViewport);
    std::shared_ptr<SViewport> GetGameViewport() const { return GameViewport; }

    void SetupSlateRenderer(std::shared_ptr<NSlateRenderer> InRenderer);

private:
    void CreatePlatformWindow(SWindow& InSlateWindow);
    
public:
    static NSlateApplication& Create();
    static void Destroy();
    static NSlateApplication& Get();
};