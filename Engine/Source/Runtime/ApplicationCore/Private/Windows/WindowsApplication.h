// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericApplication.h"

#if PLATFORM_WINDOWS

#define __ALLOW_PLATFORM_COMMON_H__
#include "Platform/PlatformCommon.h"

namespace Ayla
{
    class NWindowsApplication : public NGenericApplication
    {
    private:
        DirectoryReference EngineDirectory;

    public:
        NWindowsApplication();

        virtual std::unique_ptr<NGenericWindow> MakeWindow(const NGenericWindowDefinition& InDefinition) override;
        virtual Vector2N GetScreenResolution() override;
        virtual void PumpMessages(std::vector<NGenericPlatformInputEvent>& OutInputEvents) override;
        virtual DirectoryReference GetEngineDirectory() const override;

    public:
        static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    private:
        static std::vector<NGenericPlatformInputEvent> InputEvents;
    };
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif