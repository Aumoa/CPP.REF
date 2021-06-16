// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:WinMouse;

import std.core;
import SC.Runtime.Core;
import :MouseState;
import :MousePositionMode;
import :IFrameworkView;
import :WindowsMinimal;

using namespace std;

export class WinMouse : virtual public Object
{
public:
    WinMouse();

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
    static void ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);

    // Singleton
    static WinMouse& Get();

private:
    // Private implementation.
    class Impl;

    unique_ptr<Impl> pImpl;
};