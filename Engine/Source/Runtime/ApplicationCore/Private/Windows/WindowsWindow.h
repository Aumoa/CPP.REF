// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericWindow.h"

#if PLATFORM_WINDOWS

#include "WindowsCommon.h"

class NWindowsWindow : public NGenericWindow
{
    static std::map<uint64, String> RegisteredClasses;

private:
    NGenericWindowDefinition CachedDefinition;
    HWND hWnd = NULL;

public:
    NWindowsWindow(HINSTANCE hInstance, const NGenericWindowDefinition& InDefinition);
    virtual ~NWindowsWindow() noexcept override;

    virtual NGenericWindowDefinition GetDefinition() const override;
    virtual void* GetOSWindowHandle() const override;
    virtual void Show() override;
    virtual void Hide() override;
};

#endif