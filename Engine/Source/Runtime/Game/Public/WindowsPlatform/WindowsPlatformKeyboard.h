// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlatformMisc/IPlatformKeyboard.h"
#include <memory>

class GAME_API SWindowsPlatformKeyboard : virtual public SObject, virtual public IPlatformKeyboard
{
    GENERATED_BODY(SWindowsPlatformKeyboard)

public:
    SWindowsPlatformKeyboard();
    ~SWindowsPlatformKeyboard();

    // Retrieve the current state of the keyboard
    virtual KeyboardState GetState() const;

    // Reset the keyboard state
    virtual void Reset() noexcept;

    // Feature detection
    virtual bool IsConnected() const;

    static void ProcessMessage(uint32 message, uint64 wParam, int64 lParam);

    // Singleton
    static SWindowsPlatformKeyboard& Get();

private:
    // Private implementation.
    class Impl;
    std::unique_ptr<Impl> pImpl;
};