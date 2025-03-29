// Copyright 2020-2025 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "Windows/WindowsWindow.h"
#include "Windows/WindowsApplication.h"

namespace Ayla
{
    std::map<uint64, String> WindowsWindow::RegisteredClasses;

    WindowsWindow::WindowsWindow(HINSTANCE hInstance, const GenericWindowDefinition& InDefinition)
    {
        char PackedBuffer[1] = { (char)InDefinition.bPopup };
        uint64 HashVal = CrcHash::Hash64(PackedBuffer, sizeof(PackedBuffer));
        auto It = RegisteredClasses.find(HashVal);
        if (It == RegisteredClasses.end())
        {
            String ClassName = String::Format(TEXT("WindowsWindow_${}"), HashVal);

            WNDCLASSEXW WndClass = {};
            WndClass.cbSize = sizeof(WndClass);
            WndClass.style = CS_DBLCLKS;
            if (InDefinition.bPopup)
            {
                WndClass.style |= CS_NOCLOSE;
            }
            WndClass.lpfnWndProc = WindowsApplication::WndProc;
            WndClass.cbWndExtra = sizeof(void*);
            WndClass.hInstance = hInstance;
            WndClass.lpszClassName = ClassName.c_str();

            ATOM Reg = RegisterClassExW(&WndClass);
            checkf(Reg, TEXT("Failed to register new window class. ErrorCode: {}"), GetLastError());

            It = RegisteredClasses.emplace(HashVal, ClassName).first;
        }

        UINT WndStyle = 0;
        if (InDefinition.bPopup)
        {
            WndStyle |= WS_POPUPWINDOW;
        }
        if (InDefinition.bSystemMenu)
        {
            WndStyle |= WS_SYSMENU;
        }
        if (InDefinition.bThickframe)
        {
            WndStyle |= WS_THICKFRAME;
        }
        if (InDefinition.bSizebox)
        {
            WndStyle |= WS_MINIMIZEBOX;
            WndStyle |= WS_MAXIMIZEBOX;
        }
        if (InDefinition.bCaption)
        {
            WndStyle |= WS_CAPTION;
        }

        UINT WndStyleEx = 0;

        int nX = (int)InDefinition.DesiredScreenPosition.X;
        if (nX == -1)
        {
            nX = CW_USEDEFAULT;
        }

        int nY = (int)InDefinition.DesiredScreenPosition.Y;
        if (nY == -1)
        {
            nY = CW_USEDEFAULT;
        }

        int nWidth = (int)InDefinition.DesiredScreenSize.X;
        if (nWidth == -1)
        {
            nWidth = CW_USEDEFAULT;
        }

        int nHeight = (int)InDefinition.DesiredScreenSize.Y;
        if (nHeight == -1)
        {
            nHeight = CW_USEDEFAULT;
        }

        CachedDefinition = InDefinition;
        hWnd = CreateWindowExW(WndStyleEx, It->second.c_str(), NULL, WndStyle, nX, nY, nWidth, nHeight, NULL, NULL, hInstance, this);
    }

    WindowsWindow::~WindowsWindow() noexcept
    {
        if (hWnd != NULL)
        {
            // Ignore errors.
            SetWindowLongPtrW(hWnd, 0, (LONG_PTR)NULL);
            hWnd = NULL;
        }
    }

    GenericWindowDefinition WindowsWindow::GetDefinition() const
    {
        return CachedDefinition;
    }

    void* WindowsWindow::GetOSWindowHandle() const
    {
        return hWnd;
    }

    void WindowsWindow::Show()
    {
        ShowWindow(hWnd, SW_SHOW);
    }

    void WindowsWindow::Hide()
    {
        ShowWindow(hWnd, SW_HIDE);
    }

    Vector2N WindowsWindow::GetSize() const
    {
        RECT Rc;
        ::GetClientRect(hWnd, &Rc);
        return Vector2N(Rc.right - Rc.left, Rc.bottom - Rc.top);
    }
}

#endif