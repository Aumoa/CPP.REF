//--------------------------------------------------------------------------------------
// File: WinKeyboard.cpp
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
// http://go.microsoft.com/fwlink/?LinkID=615561
//--------------------------------------------------------------------------------------

#include "Windows/WinKeyboard.h"

static_assert(sizeof(KeyState) == (256 / 8), "Size mismatch for State");

namespace
{
    inline void KeyDown(int key, KeyState& state) noexcept
    {
        if (key < 0 || key > 0xfe)
            return;

        auto ptr = reinterpret_cast<uint32_t*>(&state);

        unsigned int bf = 1u << (key & 0x1f);
        ptr[(key >> 5)] |= bf;
    }

    inline void KeyUp(int key, KeyState& state) noexcept
    {
        if (key < 0 || key > 0xfe)
            return;

        auto ptr = reinterpret_cast<uint32_t*>(&state);

        unsigned int bf = 1u << (key & 0x1f);
        ptr[(key >> 5)] &= ~bf;
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
//         WinKeyboard::ProcessMessage(message, wParam, lParam);
//         break;
//
//     case WM_KEYDOWN:
//     case WM_SYSKEYDOWN:
//     case WM_KEYUP:
//     case WM_SYSKEYUP:
//         WinKeyboard::ProcessMessage(message, wParam, lParam);
//         break;
//
//     }
// }
//

class WinKeyboard::Impl
{
public:
    Impl(WinKeyboard* owner) :
        mState{},
        mOwner(owner)
    {
        if (s_keyboard)
        {
            throw std::logic_error("WinKeyboard is a singleton");
        }

        s_keyboard = this;
    }

    Impl(Impl&&) = default;
    Impl& operator= (Impl&&) = default;

    Impl(Impl const&) = delete;
    Impl& operator= (Impl const&) = delete;

    ~Impl()
    {
        s_keyboard = nullptr;
    }

    void GetState(KeyState& state) const
    {
        memcpy(&state, &mState, sizeof(KeyState));
    }

    void Reset() noexcept
    {
        memset(&mState, 0, sizeof(KeyState));
    }

    bool IsConnected() const
    {
        return true;
    }

    KeyState           mState;
    WinKeyboard*       mOwner;

    static WinKeyboard::Impl* s_keyboard;
};


WinKeyboard::Impl* WinKeyboard::Impl::s_keyboard = nullptr;


void WinKeyboard::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    auto pImpl = Impl::s_keyboard;

    if (!pImpl)
        return;

    bool down = false;

    switch (message)
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
WinKeyboard::WinKeyboard() noexcept(false)
    : pImpl(std::make_unique<Impl>(this))
{
}

// Public destructor.
WinKeyboard::~WinKeyboard()
{
}


KeyState WinKeyboard::GetState() const
{
    KeyState state;
    pImpl->GetState(state);
    return state;
}


void WinKeyboard::Reset() noexcept
{
    pImpl->Reset();
}


bool WinKeyboard::IsConnected() const
{
    return pImpl->IsConnected();
}

WinKeyboard& WinKeyboard::Get()
{
    if (!Impl::s_keyboard || !Impl::s_keyboard->mOwner)
        throw Exception(L"WinKeyboard singleton not created");

    return *Impl::s_keyboard->mOwner;
}