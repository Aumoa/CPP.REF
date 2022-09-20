// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

#if PLATFORM_WINDOWS

#include "PlatformMisc/IPlatformKeyboard.h"
#include "IO/KeyboardState.h"
#include "WindowsMinimal.h"
#include "WindowsKeyboard.gen.h"

SCLASS()
class WindowsKeyboard : virtual public IPlatformKeyboard
{
	GENERATED_BODY()

public:
	WindowsKeyboard();
	virtual ~WindowsKeyboard() noexcept override;

    // Retrieve the current state of the keyboard
    virtual KeyboardState GetState() override;

    // Reset the keyboard state
    virtual void Reset() override;

    // Feature detection
    virtual bool IsConnected() override;

    static void ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    // Singleton
    static WindowsKeyboard& Get();

private:
    // Private implementation.
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

#endif