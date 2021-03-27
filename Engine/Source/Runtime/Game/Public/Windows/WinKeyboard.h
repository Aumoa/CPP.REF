//--------------------------------------------------------------------------------------
// File: WinKeyboard.h
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
#include "Input/Keyboard.h"

class GAME_API WinKeyboard : virtual public Object
{
public:
    WinKeyboard();
    ~WinKeyboard() override;

    // Retrieve the current state of the keyboard
    KeyState GetState() const;

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

    std::unique_ptr<Impl> pImpl;
};