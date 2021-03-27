// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "TBitfieldInputChangeTracker.h"

enum class EKey
{
    None                = 0,

    Back                = 0x8,
    Tab                 = 0x9,

    Enter               = 0xd,

    Pause               = 0x13,
    CapsLock            = 0x14,
    Kana                = 0x15,

    Kanji               = 0x19,

    Escape              = 0x1b,
    ImeConvert          = 0x1c,
    ImeNoConvert        = 0x1d,

    Space               = 0x20,
    PageUp              = 0x21,
    PageDown            = 0x22,
    End                 = 0x23,
    Home                = 0x24,
    Left                = 0x25,
    Up                  = 0x26,
    Right               = 0x27,
    Down                = 0x28,
    Select              = 0x29,
    Print               = 0x2a,
    Execute             = 0x2b,
    PrintScreen         = 0x2c,
    Insert              = 0x2d,
    Delete              = 0x2e,
    Help                = 0x2f,
    D0                  = 0x30,
    D1                  = 0x31,
    D2                  = 0x32,
    D3                  = 0x33,
    D4                  = 0x34,
    D5                  = 0x35,
    D6                  = 0x36,
    D7                  = 0x37,
    D8                  = 0x38,
    D9                  = 0x39,

    A                   = 0x41,
    B                   = 0x42,
    C                   = 0x43,
    D                   = 0x44,
    E                   = 0x45,
    F                   = 0x46,
    G                   = 0x47,
    H                   = 0x48,
    I                   = 0x49,
    J                   = 0x4a,
    K                   = 0x4b,
    L                   = 0x4c,
    M                   = 0x4d,
    N                   = 0x4e,
    O                   = 0x4f,
    P                   = 0x50,
    Q                   = 0x51,
    R                   = 0x52,
    S                   = 0x53,
    T                   = 0x54,
    U                   = 0x55,
    V                   = 0x56,
    W                   = 0x57,
    X                   = 0x58,
    Y                   = 0x59,
    Z                   = 0x5a,
    LeftWindows         = 0x5b,
    RightWindows        = 0x5c,
    Apps                = 0x5d,

    Sleep               = 0x5f,
    NumPad0             = 0x60,
    NumPad1             = 0x61,
    NumPad2             = 0x62,
    NumPad3             = 0x63,
    NumPad4             = 0x64,
    NumPad5             = 0x65,
    NumPad6             = 0x66,
    NumPad7             = 0x67,
    NumPad8             = 0x68,
    NumPad9             = 0x69,
    Multiply            = 0x6a,
    Add                 = 0x6b,
    Separator           = 0x6c,
    Subtract            = 0x6d,

    Decimal             = 0x6e,
    Divide              = 0x6f,
    F1                  = 0x70,
    F2                  = 0x71,
    F3                  = 0x72,
    F4                  = 0x73,
    F5                  = 0x74,
    F6                  = 0x75,
    F7                  = 0x76,
    F8                  = 0x77,
    F9                  = 0x78,
    F10                 = 0x79,
    F11                 = 0x7a,
    F12                 = 0x7b,
    F13                 = 0x7c,
    F14                 = 0x7d,
    F15                 = 0x7e,
    F16                 = 0x7f,
    F17                 = 0x80,
    F18                 = 0x81,
    F19                 = 0x82,
    F20                 = 0x83,
    F21                 = 0x84,
    F22                 = 0x85,
    F23                 = 0x86,
    F24                 = 0x87,

    NumLock             = 0x90,
    Scroll              = 0x91,

    LeftShift           = 0xa0,
    RightShift          = 0xa1,
    LeftControl         = 0xa2,
    RightControl        = 0xa3,
    LeftAlt             = 0xa4,
    RightAlt            = 0xa5,
    BrowserBack         = 0xa6,
    BrowserForward      = 0xa7,
    BrowserRefresh      = 0xa8,
    BrowserStop         = 0xa9,
    BrowserSearch       = 0xaa,
    BrowserFavorites    = 0xab,
    BrowserHome         = 0xac,
    VolumeMute          = 0xad,
    VolumeDown          = 0xae,
    VolumeUp            = 0xaf,
    MediaNextTrack      = 0xb0,
    MediaPreviousTrack  = 0xb1,
    MediaStop           = 0xb2,
    MediaPlayPause      = 0xb3,
    LaunchMail          = 0xb4,
    SelectMedia         = 0xb5,
    LaunchApplication1  = 0xb6,
    LaunchApplication2  = 0xb7,

    OemSemicolon        = 0xba,
    OemPlus             = 0xbb,
    OemComma            = 0xbc,
    OemMinus            = 0xbd,
    OemPeriod           = 0xbe,
    OemQuestion         = 0xbf,
    OemTilde            = 0xc0,

    OemOpenBrackets     = 0xdb,
    OemPipe             = 0xdc,
    OemCloseBrackets    = 0xdd,
    OemQuotes           = 0xde,
    Oem8                = 0xdf,

    OemBackslash        = 0xe2,

    ProcessKey          = 0xe5,

    OemCopy             = 0xf2,
    OemAuto             = 0xf3,
    OemEnlW             = 0xf4,

    Attn                = 0xf6,
    Crsel               = 0xf7,
    Exsel               = 0xf8,
    EraseEof            = 0xf9,
    Play                = 0xfa,
    Zoom                = 0xfb,

    Pa1                 = 0xfd,
    OemClear            = 0xfe,
};

enum class EKeyEvent
{
	Pressed,
	Released,
};

struct KeyState
{
    bool bReserved0 : 8;
    bool bBack : 1;              // VK_BACK, 0x8
    bool bTab : 1;               // VK_TAB, 0x9
    bool bReserved1 : 3;
    bool bEnter : 1;             // VK_RETURN, 0xD
    bool bReserved2 : 2;
    bool bReserved3 : 3;
    bool bPause : 1;             // VK_PAUSE, 0x13
    bool bCapsLock : 1;          // VK_CAPITAL, 0x14
    bool bKana : 1;              // VK_KANA, 0x15
    bool bReserved4 : 2;
    bool bReserved5 : 1;
    bool bKanji : 1;             // VK_KANJI, 0x19
    bool bReserved6 : 1;
    bool bEscape : 1;            // VK_ESCAPE, 0x1B
    bool bImeConvert : 1;        // VK_CONVERT, 0x1C
    bool bImeNoConvert : 1;      // VK_NONCONVERT, 0x1D
    bool bReserved7 : 2;
    bool bSpace : 1;             // VK_SPACE, 0x20
    bool bPageUp : 1;            // VK_PRIOR, 0x21
    bool bPageDown : 1;          // VK_NEXT, 0x22
    bool bEnd : 1;               // VK_END, 0x23
    bool bHome : 1;              // VK_HOME, 0x24
    bool bLeft : 1;              // VK_LEFT, 0x25
    bool bUp : 1;                // VK_UP, 0x26
    bool bRight : 1;             // VK_RIGHT, 0x27
    bool bDown : 1;              // VK_DOWN, 0x28
    bool bSelect : 1;            // VK_SELECT, 0x29
    bool bPrint : 1;             // VK_PRINT, 0x2A
    bool bExecute : 1;           // VK_EXECUTE, 0x2B
    bool bPrintScreen : 1;       // VK_SNAPSHOT, 0x2C
    bool bInsert : 1;            // VK_INSERT, 0x2D
    bool bDelete : 1;            // VK_DELETE, 0x2E
    bool bHelp : 1;              // VK_HELP, 0x2F
    bool bD0 : 1;                // 0x30
    bool bD1 : 1;                // 0x31
    bool bD2 : 1;                // 0x32
    bool bD3 : 1;                // 0x33
    bool bD4 : 1;                // 0x34
    bool bD5 : 1;                // 0x35
    bool bD6 : 1;                // 0x36
    bool bD7 : 1;                // 0x37
    bool bD8 : 1;                // 0x38
    bool bD9 : 1;                // 0x39
    bool bReserved8 : 6;
    bool bReserved9 : 1;
    bool bA : 1;                 // 0x41
    bool bB : 1;                 // 0x42
    bool bC : 1;                 // 0x43
    bool bD : 1;                 // 0x44
    bool bE : 1;                 // 0x45
    bool bF : 1;                 // 0x46
    bool bG : 1;                 // 0x47
    bool bH : 1;                 // 0x48
    bool bI : 1;                 // 0x49
    bool bJ : 1;                 // 0x4A
    bool bK : 1;                 // 0x4B
    bool bL : 1;                 // 0x4C
    bool bM : 1;                 // 0x4D
    bool bN : 1;                 // 0x4E
    bool bO : 1;                 // 0x4F
    bool bP : 1;                 // 0x50
    bool bQ : 1;                 // 0x51
    bool bR : 1;                 // 0x52
    bool bS : 1;                 // 0x53
    bool bT : 1;                 // 0x54
    bool bU : 1;                 // 0x55
    bool bV : 1;                 // 0x56
    bool bW : 1;                 // 0x57
    bool bX : 1;                 // 0x58
    bool bY : 1;                 // 0x59
    bool bZ : 1;                 // 0x5A
    bool bLeftWindows : 1;       // VK_LWIN, 0x5B
    bool bRightWindows : 1;      // VK_RWIN, 0x5C
    bool bApps : 1;              // VK_APPS, 0x5D
    bool bReserved10 : 1;
    bool bSleep : 1;             // VK_SLEEP, 0x5F
    bool bNumPad0 : 1;           // VK_NUMPAD0, 0x60
    bool bNumPad1 : 1;           // VK_NUMPAD1, 0x61
    bool bNumPad2 : 1;           // VK_NUMPAD2, 0x62
    bool bNumPad3 : 1;           // VK_NUMPAD3, 0x63
    bool bNumPad4 : 1;           // VK_NUMPAD4, 0x64
    bool bNumPad5 : 1;           // VK_NUMPAD5, 0x65
    bool bNumPad6 : 1;           // VK_NUMPAD6, 0x66
    bool bNumPad7 : 1;           // VK_NUMPAD7, 0x67
    bool bNumPad8 : 1;           // VK_NUMPAD8, 0x68
    bool bNumPad9 : 1;           // VK_NUMPAD9, 0x69
    bool bMultiply : 1;          // VK_MULTIPLY, 0x6A
    bool bAdd : 1;               // VK_ADD, 0x6B
    bool bSeparator : 1;         // VK_SEPARATOR, 0x6C
    bool bSubtract : 1;          // VK_SUBTRACT, 0x6D
    bool bDecimal : 1;           // VK_DECIMANL, 0x6E
    bool bDivide : 1;            // VK_DIVIDE, 0x6F
    bool bF1 : 1;                // VK_F1, 0x70
    bool bF2 : 1;                // VK_F2, 0x71
    bool bF3 : 1;                // VK_F3, 0x72
    bool bF4 : 1;                // VK_F4, 0x73
    bool bF5 : 1;                // VK_F5, 0x74
    bool bF6 : 1;                // VK_F6, 0x75
    bool bF7 : 1;                // VK_F7, 0x76
    bool bF8 : 1;                // VK_F8, 0x77
    bool bF9 : 1;                // VK_F9, 0x78
    bool bF10 : 1;               // VK_F10, 0x79
    bool bF11 : 1;               // VK_F11, 0x7A
    bool bF12 : 1;               // VK_F12, 0x7B
    bool bF13 : 1;               // VK_F13, 0x7C
    bool bF14 : 1;               // VK_F14, 0x7D
    bool bF15 : 1;               // VK_F15, 0x7E
    bool bF16 : 1;               // VK_F16, 0x7F
    bool bF17 : 1;               // VK_F17, 0x80
    bool bF18 : 1;               // VK_F18, 0x81
    bool bF19 : 1;               // VK_F19, 0x82
    bool bF20 : 1;               // VK_F20, 0x83
    bool bF21 : 1;               // VK_F21, 0x84
    bool bF22 : 1;               // VK_F22, 0x85
    bool bF23 : 1;               // VK_F23, 0x86
    bool bF24 : 1;               // VK_F24, 0x87
    bool bReserved11 : 8;
    bool bNumLock : 1;           // VK_NUMLOCK, 0x90
    bool bScroll : 1;            // VK_SCROLL, 0x91
    bool bReserved12 : 6;
    bool bReserved13 : 8;
    bool bLeftShift : 1;         // VK_LSHIFT, 0xA0
    bool bRightShift : 1;        // VK_RSHIFT, 0xA1
    bool bLeftControl : 1;       // VK_LCONTROL, 0xA2
    bool bRightControl : 1;      // VK_RCONTROL, 0xA3
    bool bLeftAlt : 1;           // VK_LMENU, 0xA4
    bool bRightAlt : 1;          // VK_RMENU, 0xA5
    bool bBrowserBack : 1;       // VK_BROWSER_BACK, 0xA6
    bool bBrowserForward : 1;    // VK_BROWSER_FORWARD, 0xA7
    bool bBrowserRefresh : 1;    // VK_BROWSER_REFRESH, 0xA8
    bool bBrowserStop : 1;       // VK_BROWSER_STOP, 0xA9
    bool bBrowserSearch : 1;     // VK_BROWSER_SEARCH, 0xAA
    bool bBrowserFavorites : 1;  // VK_BROWSER_FAVORITES, 0xAB
    bool bBrowserHome : 1;       // VK_BROWSER_HOME, 0xAC
    bool bVolumeMute : 1;        // VK_VOLUME_MUTE, 0xAD
    bool bVolumeDown : 1;        // VK_VOLUME_DOWN, 0xAE
    bool bVolumeUp : 1;          // VK_VOLUME_UP, 0xAF
    bool bMediaNextTrack : 1;    // VK_MEDIA_NEXT_TRACK, 0xB0
    bool bMediaPreviousTrack : 1;// VK_MEDIA_PREV_TRACK, 0xB1
    bool bMediaStop : 1;         // VK_MEDIA_STOP, 0xB2
    bool bMediaPlayPause : 1;    // VK_MEDIA_PLAY_PAUSE, 0xB3
    bool bLaunchMail : 1;        // VK_LAUNCH_MAIL, 0xB4
    bool bSelectMedia : 1;       // VK_LAUNCH_MEDIA_SELECT, 0xB5
    bool bLaunchApplication1 : 1;// VK_LAUNCH_APP1, 0xB6
    bool bLaunchApplication2 : 1;// VK_LAUNCH_APP2, 0xB7
    bool bReserved14 : 2;
    bool bOemSemicolon : 1;      // VK_OEM_1, 0xBA
    bool bOemPlus : 1;           // VK_OEM_PLUS, 0xBB
    bool bOemComma : 1;          // VK_OEM_COMMA, 0xBC
    bool bOemMinus : 1;          // VK_OEM_MINUS, 0xBD
    bool bOemPeriod : 1;         // VK_OEM_PERIOD, 0xBE
    bool bOemQuestion : 1;       // VK_OEM_2, 0xBF
    bool bOemTilde : 1;          // VK_OEM_3, 0xC0
    bool bReserved15 : 7;
    bool bReserved16 : 8;
    bool bReserved17 : 8;
    bool bReserved18 : 3;
    bool bOemOpenBrackets : 1;   // VK_OEM_4, 0xDB
    bool bOemPipe : 1;           // VK_OEM_5, 0xDC
    bool bOemCloseBrackets : 1;  // VK_OEM_6, 0xDD
    bool bOemQuotes : 1;         // VK_OEM_7, 0xDE
    bool bOem8 : 1;              // VK_OEM_8, 0xDF
    bool bReserved19 : 2;
    bool bOemBackslash : 1;      // VK_OEM_102, 0xE2
    bool bReserved20 : 2;
    bool bProcessKey : 1;        // VK_PROCESSKEY, 0xE5
    bool bReserved21 : 2;
    bool bReserved22 : 8;
    bool bReserved23 : 2;
    bool bOemCopy : 1;           // 0XF2
    bool bOemAuto : 1;           // 0xF3
    bool bOemEnlW : 1;           // 0xF4
    bool bReserved24 : 1;
    bool bAttn : 1;              // VK_ATTN, 0xF6
    bool bCrsel : 1;             // VK_CRSEL, 0xF7
    bool bExsel : 1;             // VK_EXSEL, 0xF8
    bool bEraseEof : 1;          // VK_EREOF, 0xF9
    bool bPlay : 1;              // VK_PLAY, 0xFA
    bool bZoom : 1;              // VK_ZOOM, 0xFB
    bool bReserved25 : 1;
    bool bPa1 : 1;               // VK_PA1, 0xFD
    bool bOemClear : 1;          // VK_OEM_CLEAR, 0xFE
    bool bReserved26 : 1;

    inline bool IsKeyDown(EKey key) const noexcept
    {
        return IsKeyDown((int32)key);
    }

    inline bool IsKeyDown(int32 key) const noexcept
    {
        if (key <= 0xfe)
        {
            auto ptr = reinterpret_cast<const uint32_t*>(this);
            unsigned int bf = 1u << (key & 0x1f);
            return (ptr[(key >> 5)] & bf) != 0;
        }
        return false;
    }

    inline bool IsKeyUp(EKey key) const noexcept
    {
        return IsKeyUp((int32)key);
    }

    inline bool IsKeyUp(int32 key) const noexcept
    {
        if (key <= 0xfe)
        {
            auto ptr = reinterpret_cast<const uint32_t*>(this);
            unsigned int bf = 1u << (key & 0x1f);
            return (ptr[(key >> 5)] & bf) == 0;
        }
        return false;
    }
};

class GAME_API KeyStateTracker : virtual public Object
{
public:
    using Super = Object;

private:
    TBitfieldInputChangeTracker<KeyState> tracker;

public:
    KeyStateTracker();
    ~KeyStateTracker() override;

    void Update(const KeyState& inNewState);
    void Reset();
    vs_property_get_auto(KeyState, Last, tracker.Last);

    bool IsKeyPressed(EKey inKey) const;
    bool IsKeyReleased(EKey inKey) const;
};