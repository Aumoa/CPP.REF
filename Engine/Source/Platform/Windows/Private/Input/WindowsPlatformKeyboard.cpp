//--------------------------------------------------------------------------------------
// File: SWindowsPlatformKeyboard.cpp
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
// http://go.microsoft.com/fwlink/?LinkID=615561
//--------------------------------------------------------------------------------------

#include "Input/WindowsPlatformKeyboard.h"

GENERATE_BODY(SWindowsPlatformKeyboard);

static_assert(sizeof(KeyboardState) == (256 / 8), "Size mismatch for State");

namespace WindowsPlatformKeyboardInternal
{
    inline void KeyDown(int key, KeyboardState& state) noexcept
    {
        if (key < 0 || key > 0xfe)
            return;

        auto ptr = reinterpret_cast<uint32_t*>(&state);

        unsigned int bf = 1u << (key & 0x1f);
        ptr[(key >> 5)] |= bf;

        SWindowsPlatformKeyboard::Get().KeyPressed.Broadcast((EKey)key);
    }

    inline void KeyUp(int key, KeyboardState& state) noexcept
    {
        if (key < 0 || key > 0xfe)
            return;

        auto ptr = reinterpret_cast<uint32_t*>(&state);

        unsigned int bf = 1u << (key & 0x1f);
        ptr[(key >> 5)] &= ~bf;

        SWindowsPlatformKeyboard::Get().KeyReleased.Broadcast((EKey)key);
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
//         SWindowsPlatformKeyboard::ProcessMessage(message, wParam, lParam);
//         break;
//
//     case WM_KEYDOWN:
//     case WM_SYSKEYDOWN:
//     case WM_KEYUP:
//     case WM_SYSKEYUP:
//         SWindowsPlatformKeyboard::ProcessMessage(message, wParam, lParam);
//         break;
//
//     }
// }
//

class SWindowsPlatformKeyboard::Impl
{
public:
    Impl(SWindowsPlatformKeyboard* owner) :
        mState{},
        mOwner(owner)
    {
        if (s_keyboard)
        {
            throw fatal_exception("PlatformWindowsKeyboard is a singleton.");
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

    KeyboardState           mState;
    SWindowsPlatformKeyboard* mOwner;

    static SWindowsPlatformKeyboard::Impl* s_keyboard;
};


SWindowsPlatformKeyboard::Impl* SWindowsPlatformKeyboard::Impl::s_keyboard = nullptr;


void SWindowsPlatformKeyboard::ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    using namespace WindowsPlatformKeyboardInternal;

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

SWindowsPlatformKeyboard* SWindowsPlatformKeyboard::sInstance;

// Public constructor.
SWindowsPlatformKeyboard::SWindowsPlatformKeyboard()
    : pImpl(std::make_unique<Impl>(this))
{
    checkf(sInstance == nullptr, L"Singleton instance duplicated.");
    sInstance = this;
}

SWindowsPlatformKeyboard::~SWindowsPlatformKeyboard()
{
}


KeyboardState SWindowsPlatformKeyboard::GetState()
{
    KeyboardState state;
    pImpl->GetState(state);
    return state;
}


void SWindowsPlatformKeyboard::Reset()
{
    pImpl->Reset();
}


bool SWindowsPlatformKeyboard::IsConnected()
{
    return pImpl->IsConnected();
}

SWindowsPlatformKeyboard& SWindowsPlatformKeyboard::Get()
{
    return *sInstance;
}