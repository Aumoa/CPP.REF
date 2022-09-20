//--------------------------------------------------------------------------------------
// File: WindowsKeyboard.cpp
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
// http://go.microsoft.com/fwlink/?LinkID=615561
//--------------------------------------------------------------------------------------

#if PLATFORM_WINDOWS

#include "WindowsKeyboard.h"
#include "WindowsKeyboard.gen.cpp"

static_assert(sizeof(KeyboardState) == (256 / 8), "Size mismatch for State");

namespace WindowsKeyboardInternal
{
    inline void KeyDown(int key, KeyboardState& state) noexcept
    {
        if (key < 0 || key > 0xfe)
            return;

        auto ptr = reinterpret_cast<uint32_t*>(&state);

        unsigned int bf = 1u << (key & 0x1f);
        ptr[(key >> 5)] |= bf;

        WindowsKeyboard::Get().KeyPressed.Broadcast((EKey)key);
    }

    inline void KeyUp(int key, KeyboardState& state) noexcept
    {
        if (key < 0 || key > 0xfe)
            return;

        auto ptr = reinterpret_cast<uint32_t*>(&state);

        unsigned int bf = 1u << (key & 0x1f);
        ptr[(key >> 5)] &= ~bf;

        WindowsKeyboard::Get().KeyReleased.Broadcast((EKey)key);
    }
}

//======================================================================================
// Win32 desktop implementation
//======================================================================================

//
// For a Win32 desktop application, call this function from your Window Message Procedure
//
// LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
// {
//     switch (message)
//     {
//
//     case WM_ACTIVATEAPP:
//         WindowsKeyboard::ProcessMessage(message, wParam, lParam);
//         break;
//
//     case WM_KEYDOWN:
//     case WM_SYSKEYDOWN:
//     case WM_KEYUP:
//     case WM_SYSKEYUP:
//         WindowsKeyboard::ProcessMessage(message, wParam, lParam);
//         break;
//
//     }
// }
//

class WindowsKeyboard::Impl
{
public:
    Impl(WindowsKeyboard* owner) :
        mState{},
        mOwner(owner)
    {
        if (s_keyboard)
        {
            Log::Fatal(LogWindows, TEXT("PlatformWindowsKeyboard is a singleton."));
        }

        s_keyboard = this;
    }

    Impl(Impl&&) = default;
    Impl& operator= (Impl&&) = default;

    Impl(Impl const&) = delete;
    Impl& operator= (Impl const&) = delete;

    ~Impl() noexcept
    {
        s_keyboard = nullptr;
    }

    void GetState(KeyboardState& state) const
    {
        memcpy(&state, &mState, sizeof(KeyboardState));
    }

    void Reset() noexcept
    {
        memset(&mState, 0, sizeof(KeyboardState));
    }

    bool IsConnected() const
    {
        return true;
    }

    KeyboardState       mState;
    WindowsKeyboard*    mOwner;

    static WindowsKeyboard::Impl* s_keyboard;
};


WindowsKeyboard::Impl* WindowsKeyboard::Impl::s_keyboard = nullptr;


void WindowsKeyboard::ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    using namespace WindowsKeyboardInternal;

    auto pImpl = Impl::s_keyboard;

    if (!pImpl)
        return;

    bool down = false;

    switch (uMsg)
    {
    case WM_ACTIVATEAPP:
        pImpl->Reset();
        return;

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        down = true;
        break;

    case WM_KEYUP:
    case WM_SYSKEYUP:
        break;

    default:
        return;
    }

    int vk = static_cast<int>(wParam);
    switch (vk)
    {
    case VK_SHIFT:
        vk = static_cast<int>(
            MapVirtualKey((static_cast<UINT>(lParam) & 0x00ff0000) >> 16u,
                MAPVK_VSC_TO_VK_EX));
        if (!down)
        {
            // Workaround to ensure left vs. right shift get cleared when both were pressed at same time
            KeyUp(VK_LSHIFT, pImpl->mState);
            KeyUp(VK_RSHIFT, pImpl->mState);
        }
        break;

    case VK_CONTROL:
        vk = (static_cast<UINT>(lParam) & 0x01000000) ? VK_RCONTROL : VK_LCONTROL;
        break;

    case VK_MENU:
        vk = (static_cast<UINT>(lParam) & 0x01000000) ? VK_RMENU : VK_LMENU;
        break;
    }

    if (down)
    {
        KeyDown(vk, pImpl->mState);
    }
    else
    {
        KeyUp(vk, pImpl->mState);
    }
}

// Public constructor.
WindowsKeyboard::WindowsKeyboard()
    : pImpl(std::make_unique<Impl>(this))
{
}

WindowsKeyboard::~WindowsKeyboard()
{
}


KeyboardState WindowsKeyboard::GetState()
{
    KeyboardState state;
    pImpl->GetState(state);
    return state;
}


void WindowsKeyboard::Reset()
{
    pImpl->Reset();
}


bool WindowsKeyboard::IsConnected()
{
    return pImpl->IsConnected();
}

WindowsKeyboard& WindowsKeyboard::Get()
{
    if (!Impl::s_keyboard || !Impl::s_keyboard->mOwner)
    {
        Log::Fatal(LogWindows, TEXT("WindowsKeyboard singleton not created"));
    }

    return *Impl::s_keyboard->mOwner;
}

#endif