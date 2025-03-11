// Copyright 2020-2025 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "Windows/WindowsApplication.h"
#include "Windows/WindowsWindow.h"

std::vector<NGenericPlatformInputEvent> NWindowsApplication::InputEvents;

NWindowsApplication::NWindowsApplication()
{
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

void NWindowsApplication::PumpMessages(std::vector<NGenericPlatformInputEvent>& OutInputEvents)
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

    std::swap(InputEvents, OutInputEvents);
    InputEvents.clear();
}

DirectoryReference NWindowsApplication::GetEngineDirectory() const
{
    HMODULE hModule = GetModuleHandleW(L"ApplicationCore.dll");
    static WCHAR ModuleNameBuf[1024];
    DWORD Len = GetModuleFileNameW(hModule, ModuleNameBuf, 1024);
    FileReference ModuleDll = String(ModuleNameBuf, Len);
    return ModuleDll.GetDirectory().GetParent().GetParent().GetParent().GetAbsolute();
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
    case WM_MOUSEMOVE:
        {
            NGenericPlatformInputEvent& Input = InputEvents.emplace_back();
            Input.Idx = Input.IDX_MouseMove;
            NGenericPlatformInputMouseMoveEvent& MouseMove = Input.MouseMove();
            MouseMove.Location = Vector2N((int32)(int16)LOWORD(lParam), (int32)(int16)HIWORD(lParam));
        }
        break;
    case WM_LBUTTONDOWN:
        {
            NGenericPlatformInputEvent& Input = InputEvents.emplace_back();
            Input.Idx = Input.IDX_MouseButton;
            NGenericPlatformInputMouseButtonEvent& MouseButton = Input.MouseButton();
            MouseButton.ButtonType = EGenericPlatformInputMouseButtonType::Left;
            MouseButton.bUp = false;
            MouseButton.Location = Vector2N((int32)(int16)LOWORD(lParam), (int32)(int16)HIWORD(lParam));
        }
        break;
    case WM_RBUTTONDOWN:
        {
            NGenericPlatformInputEvent& Input = InputEvents.emplace_back();
            Input.Idx = Input.IDX_MouseButton;
            NGenericPlatformInputMouseButtonEvent& MouseButton = Input.MouseButton();
            MouseButton.ButtonType = EGenericPlatformInputMouseButtonType::Right;
            MouseButton.bUp = false;
            MouseButton.Location = Vector2N((int32)(int16)LOWORD(lParam), (int32)(int16)HIWORD(lParam));
        }
        break;
    case WM_MBUTTONDOWN:
        {
            NGenericPlatformInputEvent& Input = InputEvents.emplace_back();
            Input.Idx = Input.IDX_MouseButton;
            NGenericPlatformInputMouseButtonEvent& MouseButton = Input.MouseButton();
            MouseButton.ButtonType = EGenericPlatformInputMouseButtonType::Middle;
            MouseButton.bUp = false;
            MouseButton.Location = Vector2N((int32)(int16)LOWORD(lParam), (int32)(int16)HIWORD(lParam));
        }
        break;
    case WM_LBUTTONUP:
        {
            NGenericPlatformInputEvent& Input = InputEvents.emplace_back();
            Input.Idx = Input.IDX_MouseButton;
            NGenericPlatformInputMouseButtonEvent& MouseButton = Input.MouseButton();
            MouseButton.ButtonType = EGenericPlatformInputMouseButtonType::Left;
            MouseButton.bUp = true;
            MouseButton.Location = Vector2N((int32)(int16)LOWORD(lParam), (int32)(int16)HIWORD(lParam));
        }
        break;
    case WM_RBUTTONUP:
        {
            NGenericPlatformInputEvent& Input = InputEvents.emplace_back();
            Input.Idx = Input.IDX_MouseButton;
            NGenericPlatformInputMouseButtonEvent& MouseButton = Input.MouseButton();
            MouseButton.ButtonType = EGenericPlatformInputMouseButtonType::Right;
            MouseButton.bUp = true;
            MouseButton.Location = Vector2N((int32)(int16)LOWORD(lParam), (int32)(int16)HIWORD(lParam));
        }
        break;
    case WM_MBUTTONUP:
        {
            NGenericPlatformInputEvent& Input = InputEvents.emplace_back();
            Input.Idx = Input.IDX_MouseButton;
            NGenericPlatformInputMouseButtonEvent& MouseButton = Input.MouseButton();
            MouseButton.ButtonType = EGenericPlatformInputMouseButtonType::Middle;
            MouseButton.bUp = true;
            MouseButton.Location = Vector2N((int32)(int16)LOWORD(lParam), (int32)(int16)HIWORD(lParam));
        }
        break;
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