// Copyright 2020-2022 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "Windows/WindowsApplication.h"
#include "Windows/WindowsWindow.h"

NWindowsApplication::NWindowsApplication()
{
    // Setting engine directory using ApplicationCore.dll location.
    HMODULE hModule = GetModuleHandle(TEXT("ApplicationCore.dll").c_str());
    check(hModule);

    TCHAR Buf[1024];
    DWORD Len = GetModuleFileName(hModule, Buf, 1024);
    check(Len);

    // Engine/Binaries/Win64/ApplicationCore.dll
    String WorkingDirectory(Buf, (size_t)Len);
    // Engine/Binaries/Win64
    WorkingDirectory = Path::GetDirectoryName(WorkingDirectory);

    // Engine/
    WorkingDirectory = Path::Combine(WorkingDirectory, TEXT(".."), TEXT(".."), TEXT(".."));
    WorkingDirectory = Path::GetFullPath(WorkingDirectory);

    Environment::SetEngineDirectory(WorkingDirectory);
}

std::unique_ptr<NGenericWindow> NWindowsApplication::MakeWindow(const NGenericWindowDefinition& InDefinition)
{
    return std::make_unique<NWindowsWindow>((HINSTANCE)GetApplicationPointer(), InDefinition);
}

Vector2N NWindowsApplication::GetScreenResolution()
{
    int X = GetSystemMetrics(SM_CXSCREEN);
    int Y = GetSystemMetrics(SM_CYSCREEN);
    return Vector2N(X, Y);
}

void NWindowsApplication::PumpMessages()
{
    static thread_local MSG M;
    while (PeekMessage(&M, NULL, 0, 0, PM_REMOVE))
    {
        if (M.message == WM_QUIT)
        {
            QuitApplication((int32)M.wParam);
            break;
        }
        else
        {
            TranslateMessage(&M);
            DispatchMessage(&M);
        }
    }
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
                check(lPtr);
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
    return std::make_unique<NWindowsApplication>();
}

#endif