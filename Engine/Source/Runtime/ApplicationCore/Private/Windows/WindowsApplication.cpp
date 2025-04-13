// Copyright 2020-2025 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "Windows/WindowsApplication.h"
#include "Windows/WindowsWindow.h"
#include "IO/FileReference.h"

namespace Ayla
{
    std::vector<GenericPlatformInputEvent> WindowsApplication::InputEvents;

    WindowsApplication::WindowsApplication()
    {
    }

    RPtr<GenericWindow> WindowsApplication::MakeWindow(const GenericWindowDefinition& InDefinition)
    {
        return WindowsWindow::Create((HINSTANCE)GetApplicationPointer(), InDefinition);
    }

    Vector2N WindowsApplication::GetScreenResolution()
    {
        int X = GetSystemMetrics(SM_CXSCREEN);
        int Y = GetSystemMetrics(SM_CYSCREEN);
        return Vector2N(X, Y);
    }

    void WindowsApplication::PumpMessages(std::vector<GenericPlatformInputEvent>& OutInputEvents)
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

    DirectoryReference WindowsApplication::GetEngineDirectory() const
    {
        HMODULE hModule = GetModuleHandleW(L"ApplicationCore.dll");
        static WCHAR ModuleNameBuf[1024];
        DWORD Len = GetModuleFileNameW(hModule, ModuleNameBuf, 1024);
        FileReference ModuleDll = String(ModuleNameBuf, Len);
        return ModuleDll.GetDirectory().GetParent().GetParent().GetParent().GetAbsolute();
    }

    LRESULT CALLBACK WindowsApplication::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

        WindowsWindow* WindowPtr = (WindowsWindow*)GetWindowLongPtrW(hWnd, 0);

        switch (uMsg)
        {
        case WM_MOUSEMOVE:
        {
            GenericPlatformInputEvent& Input = InputEvents.emplace_back();
            Input.Idx = Input.IDX_MouseMove;
            GenericPlatformInputMouseMoveEvent& MouseMove = Input.MouseMove();
            MouseMove.Location = Vector2N((int32)(int16)LOWORD(lParam), (int32)(int16)HIWORD(lParam));
        }
        break;
        case WM_LBUTTONDOWN:
        {
            GenericPlatformInputEvent& Input = InputEvents.emplace_back();
            Input.Idx = Input.IDX_MouseButton;
            GenericPlatformInputMouseButtonEvent& MouseButton = Input.MouseButton();
            MouseButton.ButtonType = GenericPlatformInputMouseButtonType::Left;
            MouseButton.bUp = false;
            MouseButton.Location = Vector2N((int32)(int16)LOWORD(lParam), (int32)(int16)HIWORD(lParam));
        }
        break;
        case WM_RBUTTONDOWN:
        {
            GenericPlatformInputEvent& Input = InputEvents.emplace_back();
            Input.Idx = Input.IDX_MouseButton;
            GenericPlatformInputMouseButtonEvent& MouseButton = Input.MouseButton();
            MouseButton.ButtonType = GenericPlatformInputMouseButtonType::Right;
            MouseButton.bUp = false;
            MouseButton.Location = Vector2N((int32)(int16)LOWORD(lParam), (int32)(int16)HIWORD(lParam));
        }
        break;
        case WM_MBUTTONDOWN:
        {
            GenericPlatformInputEvent& Input = InputEvents.emplace_back();
            Input.Idx = Input.IDX_MouseButton;
            GenericPlatformInputMouseButtonEvent& MouseButton = Input.MouseButton();
            MouseButton.ButtonType = GenericPlatformInputMouseButtonType::Middle;
            MouseButton.bUp = false;
            MouseButton.Location = Vector2N((int32)(int16)LOWORD(lParam), (int32)(int16)HIWORD(lParam));
        }
        break;
        case WM_LBUTTONUP:
        {
            GenericPlatformInputEvent& Input = InputEvents.emplace_back();
            Input.Idx = Input.IDX_MouseButton;
            GenericPlatformInputMouseButtonEvent& MouseButton = Input.MouseButton();
            MouseButton.ButtonType = GenericPlatformInputMouseButtonType::Left;
            MouseButton.bUp = true;
            MouseButton.Location = Vector2N((int32)(int16)LOWORD(lParam), (int32)(int16)HIWORD(lParam));
        }
        break;
        case WM_RBUTTONUP:
        {
            GenericPlatformInputEvent& Input = InputEvents.emplace_back();
            Input.Idx = Input.IDX_MouseButton;
            GenericPlatformInputMouseButtonEvent& MouseButton = Input.MouseButton();
            MouseButton.ButtonType = GenericPlatformInputMouseButtonType::Right;
            MouseButton.bUp = true;
            MouseButton.Location = Vector2N((int32)(int16)LOWORD(lParam), (int32)(int16)HIWORD(lParam));
        }
        break;
        case WM_MBUTTONUP:
        {
            GenericPlatformInputEvent& Input = InputEvents.emplace_back();
            Input.Idx = Input.IDX_MouseButton;
            GenericPlatformInputMouseButtonEvent& MouseButton = Input.MouseButton();
            MouseButton.ButtonType = GenericPlatformInputMouseButtonType::Middle;
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
    RPtr<GenericApplication> GenericApplication::CreateApplication()
    {
        return New<WindowsApplication>();
    }
}

#endif