// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <memory>
#include "GameStructures.h"

struct IFrameworkView;

class GAME_API WinMouse : virtual public Object
{
    CLASS_BODY(WinMouse)

public:
    WinMouse();
    ~WinMouse();

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
    static WinMouse& Get();

private:
    // Private implementation.
    class Impl;

    std::unique_ptr<Impl> pImpl;
};