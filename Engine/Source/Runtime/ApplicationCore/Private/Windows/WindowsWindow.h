// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericWindow.h"

#if PLATFORM_WINDOWS

#define __ALLOW_PLATFORM_COMMON_H__
#include "Platform/PlatformCommon.h"
#include "WindowsWindow.gen.h"

namespace Ayla
{
    ACLASS()
    class WindowsWindow : public GenericWindow
    {
        GENERATED_BODY()

    private:
        static std::map<uint64, String> RegisteredClasses;

    private:
        GenericWindowDefinition CachedDefinition;
        HWND hWnd = NULL;

    private:
        WindowsWindow();

    public:
        virtual ~WindowsWindow() noexcept override;

        virtual GenericWindowDefinition GetDefinition() const override;
        virtual void* GetOSWindowHandle() const override;
        virtual void Show() override;
        virtual void Hide() override;
        virtual Vector2N GetSize() const override;

        static RPtr<WindowsWindow> Create(HINSTANCE hInstance, const GenericWindowDefinition& InDefinition);
    };
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif