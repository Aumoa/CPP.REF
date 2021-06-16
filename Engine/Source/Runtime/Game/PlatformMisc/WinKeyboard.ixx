// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:WinKeyboard;

import std.core;
import SC.Runtime.Core;
import :KeyboardState;
import :WindowsMinimal;

using namespace std;

export class WinKeyboard : virtual public Object
{
public:
    WinKeyboard();
    ~WinKeyboard() override;

    // Retrieve the current state of the keyboard
    KeyboardState GetState() const;

    // Reset the keyboard state
    void Reset() noexcept;

    // Feature detection
    bool IsConnected() const;

    static void ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);

    // Singleton
    static WinKeyboard& Get();

private:
    // Private implementation.
    class Impl;

    unique_ptr<Impl> pImpl;
};