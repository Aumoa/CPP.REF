// Copyright 2020-2022 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "Windows/WindowsApplication.h"
#include "Windows/WindowsWindow.h"

constexpr LogCategory LogWindowsApplication(TEXT("LogWindowsApplication"));

NWindowsApplication::NWindowsApplication(HINSTANCE hInstance)
    : hInstance(hInstance)
{
}

std::unique_ptr<NGenericWindow> NWindowsApplication::MakeWindow(const NGenericWindowDefinition& InDefinition)
{
    return std::make_unique<NWindowsWindow>(hInstance, InDefinition);
}

Vector2N NWindowsApplication::GetScreenResolution()
{
    int X = GetSystemMetrics(SM_CXSCREEN);
    int Y = GetSystemMetrics(SM_CYSCREEN);
    return Vector2N(X, Y);
}

LRESULT CALLBACK NWindowsApplication::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_NCCREATE)
    {
        auto lpCreate = (LPCREATESTRUCTW)lParam;
        if (lpCreate->lpCreateParams != NULL)
        {
            LONG_PTR lPtr = SetWindowLongPtrW(hWnd, 0, (LONG_PTR)lpCreate->lpCreateParams);
            if (lPtr == 0)
            {
                // Retry with ensure error.
                SetLastError(0);
                lPtr = SetWindowLongPtrW(hWnd, 0, (LONG_PTR)lpCreate->lpCreateParams);
                if (lPtr == 0 && GetLastError() != 0)
                {
                    Log::Fatal(LogWindowsApplication, TEXT("Failed to set window pointer to extra space. Error: {}"), GetLastError());
                }
            }
        }
    }

    NWindowsWindow* WindowPtr = (NWindowsWindow*)GetWindowLongPtrW(hWnd, 0);

    switch (uMsg)
    {
    case WM_DESTROY:
        check(WindowPtr);
        if (WindowPtr->GetDefinition().bPrimaryWindow)
        {
            PostQuitMessage(0);
        }
        break;
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

// specialize for each platforms.
std::unique_ptr<NGenericApplication> NGenericApplication::CreateApplication()
{
    return std::make_unique<NWindowsApplication>((HINSTANCE)GetModuleHandleW(NULL));
}

#endif