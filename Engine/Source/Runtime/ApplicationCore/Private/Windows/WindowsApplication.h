// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericApplication.h"

#if PLATFORM_WINDOWS

#pragma push_macro("TEXT")
#undef TEXT

#include <Windows.h>

#pragma pop_macro("TEXT")

class NWindowsApplication : public NGenericApplication
{
public:
    NWindowsApplication();

    virtual std::unique_ptr<NGenericWindow> MakeWindow(const NGenericWindowDefinition& InDefinition) override;
    virtual Vector2N GetScreenResolution() override;

public:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif