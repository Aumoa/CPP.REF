// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

#if PLATFORM_WINDOWS

#include "PlatformMisc/IPlatformMouse.h"
#include "WindowsMinimal.h"
#include "WindowsMouse.gen.h"

class WindowsWindow;

SCLASS()
class WindowsMouse : virtual public IPlatformMouse
{
    GENERATED_BODY()

private:
    static WindowsMouse* sInstance;

public:
    WindowsMouse();
    ~WindowsMouse();

    virtual MouseState GetState() override;
    virtual void ResetScrollWheelValue() override;
    virtual void SetMode(EMousePositionMode InPositionMode);
    virtual bool IsConnected() override;
    virtual bool IsVisible() override;
    virtual void SetVisible(bool bNewVisibility) override;

    void SetWindow(WindowsWindow* Application);
    static void ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    // Singleton
    static WindowsMouse& Get();

private:
    // Private implementation.
    class Impl;

    std::unique_ptr<Impl> pImpl;
};

#endif