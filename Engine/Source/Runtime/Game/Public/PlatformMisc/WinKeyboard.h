// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <memory>
#include "GameStructures.h"

class GAME_API WinKeyboard : virtual public Object
{
public:
    WinKeyboard();
    ~WinKeyboard();

    // Retrieve the current state of the keyboard
    KeyboardState GetState() const;

    // Reset the keyboard state
    void Reset() noexcept;

    // Feature detection
    bool IsConnected() const;

    static void ProcessMessage(uint32 message, uint64 wParam, int64 lParam);

    // Singleton
    static WinKeyboard& Get();

private:
    // Private implementation.
    class Impl;

    std::unique_ptr<Impl> pImpl;
};