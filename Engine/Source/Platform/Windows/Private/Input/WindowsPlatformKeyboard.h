// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "WindowsIncludes.h"

namespace libty::inline Windows
{
    class SWindowsPlatformKeyboard : implements(SObject, IPlatformKeyboard)
    {
        GENERATED_BODY(SWindowsPlatformKeyboard)

    private:
        static SWindowsPlatformKeyboard* sInstance;

    public:
        SWindowsPlatformKeyboard();
        ~SWindowsPlatformKeyboard();

        // Retrieve the current state of the keyboard
        virtual KeyboardState GetState() override;

        // Reset the keyboard state
        virtual void Reset() override;

        // Feature detection
        virtual bool IsConnected() override;

        static void ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

        // Singleton
        static SWindowsPlatformKeyboard& Get();

    private:
        // Private implementation.
        class Impl;
        std::unique_ptr<Impl> pImpl;
    };
}