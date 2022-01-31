//--------------------------------------------------------------------------------------
// File: SWindowsPlatformMouse.cpp
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
// http://go.microsoft.com/fwlink/?LinkID=615561
//--------------------------------------------------------------------------------------

#include "Input/WindowsPlatformMouse.h"
#include "WindowsApplication.h"

GENERATE_BODY(SWindowsPlatformMouse);

namespace WindowsPlatformMouseInternal
{
    struct handle_closer { void operator()(HANDLE h) noexcept { if (h) CloseHandle(h); } };
    using ScopedHandle = std::unique_ptr<void, handle_closer>;
}

//======================================================================================
// Win32 desktop implementation
//======================================================================================

//
// For a Win32 desktop application, in your window setup be sure to call this method:
//
// m_mouse->SetWindow(hwnd);
//
// And call this static function from your Window Message Procedure
//
// LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
// {
//     switch (message)
//     {
//     case WM_ACTIVATEAPP:
//     case WM_INPUT:
//     case WM_MOUSEMOVE:
//     case WM_LBUTTONDOWN:
//     case WM_LBUTTONUP:
//     case WM_RBUTTONDOWN:
//     case WM_RBUTTONUP:
//     case WM_MBUTTONDOWN:
//     case WM_MBUTTONUP:
//     case WM_MOUSEWHEEL:
//     case WM_XBUTTONDOWN:
//     case WM_XBUTTONUP:
//     case WM_MOUSEHOVER:
//         SWindowsPlatformMouse::ProcessMessage(message, wParam, lParam);
//         break;
//
//     }
// }
//

class SWindowsPlatformMouse::Impl
{
public:
    explicit Impl(SWindowsPlatformMouse* owner) noexcept(false) :
        mState{},
        mOwner(owner),
        mWindow(nullptr),
        mMode(EMousePositionMode::Absolute),
        mLastX(0),
        mLastY(0),
        mRelativeX(0xFFFFFFFF),
        mRelativeY(0xFFFFFFFF),
        mInFocus(true)
    {
        if (s_mouse)
        {
            SE_LOG(LogWindows, Fatal, L"WindowsPlatformMouse is a singleton");
        }

        s_mouse = this;

        mScrollWheelValue.reset(CreateEventEx(nullptr, nullptr, CREATE_EVENT_MANUAL_RESET, EVENT_MODIFY_STATE | SYNCHRONIZE));
        mRelativeRead.reset(CreateEventEx(nullptr, nullptr, CREATE_EVENT_MANUAL_RESET, EVENT_MODIFY_STATE | SYNCHRONIZE));
        mAbsoluteMode.reset(CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE));
        mRelativeMode.reset(CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE));
        if (!mScrollWheelValue
            || !mRelativeRead
            || !mAbsoluteMode
            || !mRelativeMode)
        {
            throw std::system_error(std::error_code(static_cast<int>(GetLastError()), std::system_category()), "CreateEventEx");
        }
    }

    Impl(Impl&&) = default;
    Impl& operator= (Impl&&) = default;

    Impl(Impl const&) = delete;
    Impl& operator= (Impl const&) = delete;

    ~Impl()
    {
        s_mouse = nullptr;
    }

    void GetState(MouseState& state) const
    {
        memcpy(&state, &mState, sizeof(MouseState));
        state.Mode = mMode;

        DWORD result = WaitForSingleObjectEx(mScrollWheelValue.get(), 0, FALSE);
        if (result == WAIT_FAILED)
            throw std::system_error(std::error_code(static_cast<int>(GetLastError()), std::system_category()), "WaitForSingleObjectEx");

        if (result == WAIT_OBJECT_0)
        {
            state.ScrollWheelValue = 0;
        }

        if (state.Mode == EMousePositionMode::Relative)
        {
            result = WaitForSingleObjectEx(mRelativeRead.get(), 0, FALSE);

            if (result == WAIT_FAILED)
                throw std::system_error(std::error_code(static_cast<int>(GetLastError()), std::system_category()), "WaitForSingleObjectEx");

            if (result == WAIT_OBJECT_0)
            {
                state.X = 0;
                state.Y = 0;
            }
            else
            {
                SetEvent(mRelativeRead.get());
            }
        }
    }

    void ResetScrollWheelValue() noexcept
    {
        SetEvent(mScrollWheelValue.get());
    }

    void SetMode(EMousePositionMode mode)
    {
        if (mMode == mode)
            return;

        SetEvent((mode == EMousePositionMode::Absolute) ? mAbsoluteMode.get() : mRelativeMode.get());

        checkf(mWindow, L"Internal logic error.");

        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.dwFlags = TME_HOVER;
        tme.hwndTrack = mWindow;
        tme.dwHoverTime = 1;
        if (!TrackMouseEvent(&tme))
        {
            throw std::system_error(std::error_code(static_cast<int>(GetLastError()), std::system_category()), "TrackMouseEvent");
        }
    }

    bool IsConnected() const noexcept
    {
        return GetSystemMetrics(SM_MOUSEPRESENT) != 0;
    }

    bool IsVisible() const noexcept
    {
        if (mMode == EMousePositionMode::Relative)
            return false;

        CURSORINFO info = { sizeof(CURSORINFO), 0, nullptr, {} };
        if (!GetCursorInfo(&info))
            return false;

        return (info.flags & CURSOR_SHOWING) != 0;
    }

    void SetVisible(bool visible)
    {
        if (mMode == EMousePositionMode::Relative)
            return;

        CURSORINFO info = { sizeof(CURSORINFO), 0, nullptr, {} };
        if (!GetCursorInfo(&info))
        {
            throw std::system_error(std::error_code(static_cast<int>(GetLastError()), std::system_category()), "GetCursorInfo");
        }

        bool isvisible = (info.flags & CURSOR_SHOWING) != 0;
        if (isvisible != visible)
        {
            ShowCursor(visible);
        }
    }

    void SetWindow(HWND window)
    {
        if (mWindow == window)
            return;

        checkf(window != nullptr, L"Internal logic error.");

        RAWINPUTDEVICE Rid;
        Rid.usUsagePage = 0x1 /* HID_USAGE_PAGE_GENERIC */;
        Rid.usUsage = 0x2 /* HID_USAGE_GENERIC_MOUSE */;
        Rid.dwFlags = RIDEV_INPUTSINK;
        Rid.hwndTarget = window;
        if (!RegisterRawInputDevices(&Rid, 1, sizeof(RAWINPUTDEVICE)))
        {
            throw std::system_error(std::error_code(static_cast<int>(GetLastError()), std::system_category()), "RegisterRawInputDevices");
        }

        mWindow = window;
    }

    MouseState      mState;

    SWindowsPlatformMouse* mOwner;

    static SWindowsPlatformMouse::Impl* s_mouse;

private:
    HWND            mWindow;
    EMousePositionMode mMode;

    WindowsPlatformMouseInternal::ScopedHandle    mScrollWheelValue;
    WindowsPlatformMouseInternal::ScopedHandle    mRelativeRead;
    WindowsPlatformMouseInternal::ScopedHandle    mAbsoluteMode;
    WindowsPlatformMouseInternal::ScopedHandle    mRelativeMode;

    int             mLastX;
    int             mLastY;
    int             mRelativeX;
    int             mRelativeY;

    bool            mInFocus;

    friend void SWindowsPlatformMouse::ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    void ClipToWindow() noexcept
    {
        checkf(mWindow != nullptr, L"internal logic error.");

        RECT rect;
        GetClientRect(mWindow, &rect);

        POINT ul;
        ul.x = rect.left;
        ul.y = rect.top;

        POINT lr;
        lr.x = rect.right;
        lr.y = rect.bottom;

        MapWindowPoints(mWindow, nullptr, &ul, 1);
        MapWindowPoints(mWindow, nullptr, &lr, 1);

        rect.left = ul.x;
        rect.top = ul.y;

        rect.right = lr.x;
        rect.bottom = lr.y;

        ClipCursor(&rect);
    }
};


SWindowsPlatformMouse::Impl* SWindowsPlatformMouse::Impl::s_mouse = nullptr;
SWindowsPlatformMouse* SWindowsPlatformMouse::sInstance;


void SWindowsPlatformMouse::SetWindow(SWindowsApplication* Application)
{
    pImpl->SetWindow((HWND)Application->GetWindowHandle());
}


void SWindowsPlatformMouse::ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    auto pImpl = Impl::s_mouse;

    if (!pImpl)
        return;

    HANDLE events[3] = { pImpl->mScrollWheelValue.get(), pImpl->mAbsoluteMode.get(), pImpl->mRelativeMode.get() };
    switch (WaitForMultipleObjectsEx(static_cast<DWORD>(std::size(events)), events, FALSE, 0, FALSE))
    {
    default:
    case WAIT_TIMEOUT:
        break;

    case WAIT_OBJECT_0:
        pImpl->mState.ScrollWheelValue = 0;
        ResetEvent(events[0]);
        break;

    case (WAIT_OBJECT_0 + 1):
    {
        pImpl->mMode = EMousePositionMode::Absolute;
        ClipCursor(nullptr);

        POINT point;
        point.x = pImpl->mLastX;
        point.y = pImpl->mLastY;

        // We show the cursor before moving it to support Remote Desktop
        ShowCursor(TRUE);

        if (MapWindowPoints(pImpl->mWindow, nullptr, &point, 1))
        {
            SetCursorPos(point.x, point.y);
        }
        pImpl->mState.X = pImpl->mLastX;
        pImpl->mState.Y = pImpl->mLastY;
    }
    break;

    case (WAIT_OBJECT_0 + 2):
    {
        ResetEvent(pImpl->mRelativeRead.get());

        pImpl->mMode = EMousePositionMode::Relative;
        pImpl->mState.X = pImpl->mState.Y = 0;
        pImpl->mRelativeX = 0xFFFFFFFF;
        pImpl->mRelativeY = 0xFFFFFFFF;

        ShowCursor(FALSE);

        pImpl->ClipToWindow();
    }
    break;

    case WAIT_FAILED:
        throw std::system_error(std::error_code(static_cast<int>(GetLastError()), std::system_category()), "WaitForMultipleObjectsEx");
    }

    // All mouse messages provide a new pointer position
    int xPos = static_cast<short>(LOWORD(lParam)); // GET_X_LPARAM(lParam);
    int yPos = static_cast<short>(HIWORD(lParam)); // GET_Y_LPARAM(lParam);

    switch (uMsg)
    {
    case WM_ACTIVATEAPP:
        if (wParam)
        {
            pImpl->mInFocus = true;

            if (pImpl->mMode == EMousePositionMode::Relative)
            {
                pImpl->mState.X = pImpl->mState.Y = 0;

                ShowCursor(FALSE);

                pImpl->ClipToWindow();
            }
        }
        else
        {
            int scrollWheel = pImpl->mState.ScrollWheelValue;
            memset(&pImpl->mState, 0, sizeof(MouseState));
            pImpl->mState.ScrollWheelValue = scrollWheel;

            pImpl->mInFocus = false;
        }
        return;

    case WM_INPUT:
        if (pImpl->mInFocus && pImpl->mMode == EMousePositionMode::Relative)
        {
            RAWINPUT raw;
            UINT rawSize = sizeof(raw);

            UINT resultData = GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, &raw, &rawSize, sizeof(RAWINPUTHEADER));
            if (resultData == UINT(-1))
            {
                throw std::runtime_error("GetRawInputData");
            }

            if (raw.header.dwType == RIM_TYPEMOUSE)
            {
                if (!(raw.data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE))
                {
                    pImpl->mState.X = raw.data.mouse.lLastX;
                    pImpl->mState.Y = raw.data.mouse.lLastY;

                    ResetEvent(pImpl->mRelativeRead.get());
                }
                else if (raw.data.mouse.usFlags & MOUSE_VIRTUAL_DESKTOP)
                {
                    // This is used to make Remote Desktop sessons work
                    const int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
                    const int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);

                    int x = static_cast<int>((float(raw.data.mouse.lLastX) / 65535.0f) * float(width));
                    int y = static_cast<int>((float(raw.data.mouse.lLastY) / 65535.0f) * float(height));

                    if (pImpl->mRelativeX == 0xFFFFFFFF)
                    {
                        pImpl->mState.X = pImpl->mState.Y = 0;
                    }
                    else
                    {
                        pImpl->mState.X = x - pImpl->mRelativeX;
                        pImpl->mState.Y = y - pImpl->mRelativeY;
                    }

                    pImpl->mRelativeX = x;
                    pImpl->mRelativeY = y;

                    ResetEvent(pImpl->mRelativeRead.get());
                }
            }
        }
        return;

    case WM_MOUSEMOVE:
        Get().CursorMoved.Broadcast(Vector2N(xPos, yPos));
        break;

    case WM_LBUTTONDOWN:
        pImpl->mState.bLeftButton = true;
        Get().MouseButtonPressed.Broadcast(Vector2N(xPos, yPos), EMouseButton::Left);
        break;

    case WM_LBUTTONUP:
        pImpl->mState.bLeftButton = false;
        Get().MouseButtonReleased.Broadcast(Vector2N(xPos, yPos), EMouseButton::Left);
        break;

    case WM_RBUTTONDOWN:
        pImpl->mState.bRightButton = true;
        Get().MouseButtonPressed.Broadcast(Vector2N(xPos, yPos), EMouseButton::Right);
        break;

    case WM_RBUTTONUP:
        pImpl->mState.bRightButton = false;
        Get().MouseButtonReleased.Broadcast(Vector2N(xPos, yPos), EMouseButton::Right);
        break;

    case WM_MBUTTONDOWN:
        pImpl->mState.bMiddleButton = true;
        Get().MouseButtonPressed.Broadcast(Vector2N(xPos, yPos), EMouseButton::Middle);
        break;

    case WM_MBUTTONUP:
        pImpl->mState.bMiddleButton = false;
        Get().MouseButtonReleased.Broadcast(Vector2N(xPos, yPos), EMouseButton::Middle);
        break;

    case WM_MOUSEWHEEL:
        pImpl->mState.ScrollWheelValue += GET_WHEEL_DELTA_WPARAM(wParam);
        Get().MouseWheelScrolled.Broadcast(GET_WHEEL_DELTA_WPARAM(wParam));
        return;

    case WM_XBUTTONDOWN:
        switch (GET_XBUTTON_WPARAM(wParam))
        {
        case XBUTTON1:
            pImpl->mState.bXButton1 = true;
            Get().MouseButtonPressed.Broadcast(Vector2N(xPos, yPos), EMouseButton::X1);
            break;

        case XBUTTON2:
            pImpl->mState.bXButton2 = true;
            Get().MouseButtonPressed.Broadcast(Vector2N(xPos, yPos), EMouseButton::X2);
            break;
        }
        break;

    case WM_XBUTTONUP:
        switch (GET_XBUTTON_WPARAM(wParam))
        {
        case XBUTTON1:
            pImpl->mState.bXButton1 = false;
            Get().MouseButtonReleased.Broadcast(Vector2N(xPos, yPos), EMouseButton::X1);
            break;

        case XBUTTON2:
            pImpl->mState.bXButton2 = false;
            Get().MouseButtonReleased.Broadcast(Vector2N(xPos, yPos), EMouseButton::X2);
            break;
        }
        break;

    case WM_MOUSEHOVER:
        break;

    default:
        // Not a mouse message, so exit
        return;
    }

    if (pImpl->mMode == EMousePositionMode::Absolute)
    {
        pImpl->mState.X = pImpl->mLastX = xPos;
        pImpl->mState.Y = pImpl->mLastY = yPos;
    }
}

#pragma warning( disable : 4355 )

// Public constructor.
SWindowsPlatformMouse::SWindowsPlatformMouse()
    : pImpl(std::make_unique<Impl>(this))
{
    checkf(sInstance == nullptr, L"Internal logic error.");
    sInstance = this;
}

SWindowsPlatformMouse::~SWindowsPlatformMouse()
{
}


MouseState SWindowsPlatformMouse::GetState()
{
    MouseState state;
    pImpl->GetState(state);
    return state;
}


void SWindowsPlatformMouse::ResetScrollWheelValue()
{
    pImpl->ResetScrollWheelValue();
}


void SWindowsPlatformMouse::SetMode(EMousePositionMode mode)
{
    pImpl->SetMode(mode);
}


bool SWindowsPlatformMouse::IsConnected()
{
    return pImpl->IsConnected();
}

bool SWindowsPlatformMouse::IsVisible()
{
    return pImpl->IsVisible();
}

void SWindowsPlatformMouse::SetVisible(bool visible)
{
    pImpl->SetVisible(visible);
}

SWindowsPlatformMouse& SWindowsPlatformMouse::Get()
{
    return *sInstance;
}