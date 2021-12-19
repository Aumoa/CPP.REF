// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Input/IPlatformMouse.h"
#include "WindowsIncludes.h"

class SWindowsApplication;

class SWindowsPlatformMouse : implements SObject, implements IPlatformMouse
{
    GENERATED_BODY(SWindowsPlatformMouse)

private:
    static SWindowsPlatformMouse* sInstance;

public:
    SWindowsPlatformMouse();
    ~SWindowsPlatformMouse();

    virtual MouseState GetState() override;
    virtual void ResetScrollWheelValue() override;
    virtual void SetMode(EMousePositionMode InPositionMode);
    virtual bool IsConnected() override;
    virtual bool IsVisible() override;
    virtual void SetVisible(bool bNewVisibility) override;

    void SetWindow(SWindowsApplication* Application);
    static void ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    // Singleton
    static SWindowsPlatformMouse& Get();

private:
    // Private implementation.
    class Impl;

    std::unique_ptr<Impl> pImpl;
};