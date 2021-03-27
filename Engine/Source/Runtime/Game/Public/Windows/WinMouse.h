//--------------------------------------------------------------------------------------
// File: WinMouse.h
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
// http://go.microsoft.com/fwlink/?LinkID=615561
//--------------------------------------------------------------------------------------

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "WindowsMinimal.h"
#include "Input/Mouse.h"

class WinMouse : virtual public Object
{
public:
    WinMouse() noexcept(false);
    ~WinMouse() override;

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

    void SetWindow(HWND window);
    static void ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);

    // Singleton
    static WinMouse& Get();

private:
    // Private implementation.
    class Impl;

    std::unique_ptr<Impl> pImpl;
};