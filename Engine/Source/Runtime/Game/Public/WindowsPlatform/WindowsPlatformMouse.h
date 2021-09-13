// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlatformMisc/IPlatformMouse.h"
#include <memory>

struct IFrameworkView;

class GAME_API SWindowsPlatformMouse : virtual public SObject, virtual public IPlatformMouse
{
    GENERATED_BODY(SWindowsPlatformMouse)

public:
    SWindowsPlatformMouse();
    ~SWindowsPlatformMouse();

    // Retrieve the current state of the mouse
    MouseState GetState() const;

    // Resets the accumulated scroll wheel value
    void ResetScrollWheelValue() noexcept;

    // Sets mouse mode (defaults to absolute)
    void SetMode(EMousePositionMode mode);

    // Feature detection
    bool IsConnected() const;

    // Cursor visibility
    bool IsVisible() const noexcept;
    void SetVisible(bool visible);

    void SetWindow(IFrameworkView* window);
    static void ProcessMessage(uint32 message, uint64 wParam, int64 lParam);

    // Singleton
    static SWindowsPlatformMouse& Get();

private:
    // Private implementation.
    class Impl;

    std::unique_ptr<Impl> pImpl;
};