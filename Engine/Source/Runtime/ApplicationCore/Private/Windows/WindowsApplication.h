// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericApplication.h"

#if PLATFORM_WINDOWS

#include "WindowsCommon.h"

class NWindowsApplication : public NGenericApplication
{
public:
    NWindowsApplication();

    virtual std::unique_ptr<NGenericWindow> MakeWindow(const NGenericWindowDefinition& InDefinition) override;
    virtual Vector2N GetScreenResolution() override;
    virtual void PumpMessages() override;

public:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif