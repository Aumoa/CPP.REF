// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericApplication.h"
#include "WindowsApplication.gen.h"

#if PLATFORM_WINDOWS

#define __ALLOW_PLATFORM_COMMON_H__
#include "Platform/PlatformCommon.h"

namespace Ayla
{
    ACLASS()
    class WindowsApplication : public GenericApplication
    {
        GENERATED_BODY()

    private:
        DirectoryReference EngineDirectory;

    public:
        WindowsApplication();

        virtual RPtr<GenericWindow> MakeWindow(const GenericWindowDefinition& InDefinition) override;
        virtual Vector2N GetScreenResolution() override;
        virtual void PumpMessages(std::vector<GenericPlatformInputEvent>& OutInputEvents) override;
        virtual DirectoryReference GetEngineDirectory() const override;

    public:
        static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    private:
        static std::vector<GenericPlatformInputEvent> InputEvents;
    };
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif