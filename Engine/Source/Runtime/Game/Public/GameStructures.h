// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEnums.h"
#include "RHI/RHIStructures.h"

class SVertexFactory;
class SMaterial;

struct MeshBatchElement
{
	uint32 IndexCount = 0;
	uint32 InstanceCount = 0;
	int32 StartIndexLocation = 0;
	int32 BaseVertexLocation = 0;
	uint32 VertexCount = 0;
    uint32 MaterialSlotIndex = 0;
};

struct MeshBatch
{
    std::vector<RHIVertex> VertexBuffer;
    std::vector<uint32> IndexBuffer;
    uint64 VertexBufferLocation = 0;
    uint64 IndexBufferLocation = 0;

	std::vector<MeshBatchElement> Elements;
    std::vector<SMaterial*> MaterialSlots;
    SVertexFactory* VertexFactory = nullptr;
};

struct MinimalViewInfo
{
    uint8 bInit : 1 = false;
	Vector3 Location;
	Quaternion Rotation;
	float AspectRatio = 1.0f;
    Degrees FOVAngle;
    float NearPlane;
    float FarPlane;
};

struct KeyboardState
{
    uint8 bReserved0 : 8;
    uint8 bBack : 1;              // VK_BACK, 0x8
    uint8 bTab : 1;               // VK_TAB, 0x9
    uint8 bReserved1 : 3;
    uint8 bEnter : 1;             // VK_RETURN, 0xD
    uint8 bReserved2 : 2;
    uint8 bReserved3 : 3;
    uint8 bPause : 1;             // VK_PAUSE, 0x13
    uint8 bCapsLock : 1;          // VK_CAPITAL, 0x14
    uint8 bKana : 1;              // VK_KANA, 0x15
    uint8 bReserved4 : 2;
    uint8 bReserved5 : 1;
    uint8 bKanji : 1;             // VK_KANJI, 0x19
    uint8 bReserved6 : 1;
    uint8 bEscape : 1;            // VK_ESCAPE, 0x1B
    uint8 bImeConvert : 1;        // VK_CONVERT, 0x1C
    uint8 bImeNoConvert : 1;      // VK_NONCONVERT, 0x1D
    uint8 bReserved7 : 2;
    uint8 bSpace : 1;             // VK_SPACE, 0x20
    uint8 bPageUp : 1;            // VK_PRIOR, 0x21
    uint8 bPageDown : 1;          // VK_NEXT, 0x22
    uint8 bEnd : 1;               // VK_END, 0x23
    uint8 bHome : 1;              // VK_HOME, 0x24
    uint8 bLeft : 1;              // VK_LEFT, 0x25
    uint8 bUp : 1;                // VK_UP, 0x26
    uint8 bRight : 1;             // VK_RIGHT, 0x27
    uint8 bDown : 1;              // VK_DOWN, 0x28
    uint8 bSelect : 1;            // VK_SELECT, 0x29
    uint8 bPrint : 1;             // VK_PRINT, 0x2A
    uint8 bExecute : 1;           // VK_EXECUTE, 0x2B
    uint8 bPrintScreen : 1;       // VK_SNAPSHOT, 0x2C
    uint8 bInsert : 1;            // VK_INSERT, 0x2D
    uint8 bDelete : 1;            // VK_DELETE, 0x2E
    uint8 bHelp : 1;              // VK_HELP, 0x2F
    uint8 bD0 : 1;                // 0x30
    uint8 bD1 : 1;                // 0x31
    uint8 bD2 : 1;                // 0x32
    uint8 bD3 : 1;                // 0x33
    uint8 bD4 : 1;                // 0x34
    uint8 bD5 : 1;                // 0x35
    uint8 bD6 : 1;                // 0x36
    uint8 bD7 : 1;                // 0x37
    uint8 bD8 : 1;                // 0x38
    uint8 bD9 : 1;                // 0x39
    uint8 bReserved8 : 6;
    uint8 bReserved9 : 1;
    uint8 bA : 1;                 // 0x41
    uint8 bB : 1;                 // 0x42
    uint8 bC : 1;                 // 0x43
    uint8 bD : 1;                 // 0x44
    uint8 bE : 1;                 // 0x45
    uint8 bF : 1;                 // 0x46
    uint8 bG : 1;                 // 0x47
    uint8 bH : 1;                 // 0x48
    uint8 bI : 1;                 // 0x49
    uint8 bJ : 1;                 // 0x4A
    uint8 bK : 1;                 // 0x4B
    uint8 bL : 1;                 // 0x4C
    uint8 bM : 1;                 // 0x4D
    uint8 bN : 1;                 // 0x4E
    uint8 bO : 1;                 // 0x4F
    uint8 bP : 1;                 // 0x50
    uint8 bQ : 1;                 // 0x51
    uint8 bR : 1;                 // 0x52
    uint8 bS : 1;                 // 0x53
    uint8 bT : 1;                 // 0x54
    uint8 bU : 1;                 // 0x55
    uint8 bV : 1;                 // 0x56
    uint8 bW : 1;                 // 0x57
    uint8 bX : 1;                 // 0x58
    uint8 bY : 1;                 // 0x59
    uint8 bZ : 1;                 // 0x5A
    uint8 bLeftWindows : 1;       // VK_LWIN, 0x5B
    uint8 bRightWindows : 1;      // VK_RWIN, 0x5C
    uint8 bApps : 1;              // VK_APPS, 0x5D
    uint8 bReserved10 : 1;
    uint8 bSleep : 1;             // VK_SLEEP, 0x5F
    uint8 bNumPad0 : 1;           // VK_NUMPAD0, 0x60
    uint8 bNumPad1 : 1;           // VK_NUMPAD1, 0x61
    uint8 bNumPad2 : 1;           // VK_NUMPAD2, 0x62
    uint8 bNumPad3 : 1;           // VK_NUMPAD3, 0x63
    uint8 bNumPad4 : 1;           // VK_NUMPAD4, 0x64
    uint8 bNumPad5 : 1;           // VK_NUMPAD5, 0x65
    uint8 bNumPad6 : 1;           // VK_NUMPAD6, 0x66
    uint8 bNumPad7 : 1;           // VK_NUMPAD7, 0x67
    uint8 bNumPad8 : 1;           // VK_NUMPAD8, 0x68
    uint8 bNumPad9 : 1;           // VK_NUMPAD9, 0x69
    uint8 bMultiply : 1;          // VK_MULTIPLY, 0x6A
    uint8 bAdd : 1;               // VK_ADD, 0x6B
    uint8 bSeparator : 1;         // VK_SEPARATOR, 0x6C
    uint8 bSubtract : 1;          // VK_SUBTRACT, 0x6D
    uint8 bDecimal : 1;           // VK_DECIMANL, 0x6E
    uint8 bDivide : 1;            // VK_DIVIDE, 0x6F
    uint8 bF1 : 1;                // VK_F1, 0x70
    uint8 bF2 : 1;                // VK_F2, 0x71
    uint8 bF3 : 1;                // VK_F3, 0x72
    uint8 bF4 : 1;                // VK_F4, 0x73
    uint8 bF5 : 1;                // VK_F5, 0x74
    uint8 bF6 : 1;                // VK_F6, 0x75
    uint8 bF7 : 1;                // VK_F7, 0x76
    uint8 bF8 : 1;                // VK_F8, 0x77
    uint8 bF9 : 1;                // VK_F9, 0x78
    uint8 bF10 : 1;               // VK_F10, 0x79
    uint8 bF11 : 1;               // VK_F11, 0x7A
    uint8 bF12 : 1;               // VK_F12, 0x7B
    uint8 bF13 : 1;               // VK_F13, 0x7C
    uint8 bF14 : 1;               // VK_F14, 0x7D
    uint8 bF15 : 1;               // VK_F15, 0x7E
    uint8 bF16 : 1;               // VK_F16, 0x7F
    uint8 bF17 : 1;               // VK_F17, 0x80
    uint8 bF18 : 1;               // VK_F18, 0x81
    uint8 bF19 : 1;               // VK_F19, 0x82
    uint8 bF20 : 1;               // VK_F20, 0x83
    uint8 bF21 : 1;               // VK_F21, 0x84
    uint8 bF22 : 1;               // VK_F22, 0x85
    uint8 bF23 : 1;               // VK_F23, 0x86
    uint8 bF24 : 1;               // VK_F24, 0x87
    uint8 bReserved11 : 8;
    uint8 bNumLock : 1;           // VK_NUMLOCK, 0x90
    uint8 bScroll : 1;            // VK_SCROLL, 0x91
    uint8 bReserved12 : 6;
    uint8 bReserved13 : 8;
    uint8 bLeftShift : 1;         // VK_LSHIFT, 0xA0
    uint8 bRightShift : 1;        // VK_RSHIFT, 0xA1
    uint8 bLeftControl : 1;       // VK_LCONTROL, 0xA2
    uint8 bRightControl : 1;      // VK_RCONTROL, 0xA3
    uint8 bLeftAlt : 1;           // VK_LMENU, 0xA4
    uint8 bRightAlt : 1;          // VK_RMENU, 0xA5
    uint8 bBrowserBack : 1;       // VK_BROWSER_BACK, 0xA6
    uint8 bBrowserForward : 1;    // VK_BROWSER_FORWARD, 0xA7
    uint8 bBrowserRefresh : 1;    // VK_BROWSER_REFRESH, 0xA8
    uint8 bBrowserStop : 1;       // VK_BROWSER_STOP, 0xA9
    uint8 bBrowserSearch : 1;     // VK_BROWSER_SEARCH, 0xAA
    uint8 bBrowserFavorites : 1;  // VK_BROWSER_FAVORITES, 0xAB
    uint8 bBrowserHome : 1;       // VK_BROWSER_HOME, 0xAC
    uint8 bVolumeMute : 1;        // VK_VOLUME_MUTE, 0xAD
    uint8 bVolumeDown : 1;        // VK_VOLUME_DOWN, 0xAE
    uint8 bVolumeUp : 1;          // VK_VOLUME_UP, 0xAF
    uint8 bMediaNextTrack : 1;    // VK_MEDIA_NEXT_TRACK, 0xB0
    uint8 bMediaPreviousTrack : 1;// VK_MEDIA_PREV_TRACK, 0xB1
    uint8 bMediaStop : 1;         // VK_MEDIA_STOP, 0xB2
    uint8 bMediaPlayPause : 1;    // VK_MEDIA_PLAY_PAUSE, 0xB3
    uint8 bLaunchMail : 1;        // VK_LAUNCH_MAIL, 0xB4
    uint8 bSelectMedia : 1;       // VK_LAUNCH_MEDIA_SELECT, 0xB5
    uint8 bLaunchApplication1 : 1;// VK_LAUNCH_APP1, 0xB6
    uint8 bLaunchApplication2 : 1;// VK_LAUNCH_APP2, 0xB7
    uint8 bReserved14 : 2;
    uint8 bOemSemicolon : 1;      // VK_OEM_1, 0xBA
    uint8 bOemPlus : 1;           // VK_OEM_PLUS, 0xBB
    uint8 bOemComma : 1;          // VK_OEM_COMMA, 0xBC
    uint8 bOemMinus : 1;          // VK_OEM_MINUS, 0xBD
    uint8 bOemPeriod : 1;         // VK_OEM_PERIOD, 0xBE
    uint8 bOemQuestion : 1;       // VK_OEM_2, 0xBF
    uint8 bOemTilde : 1;          // VK_OEM_3, 0xC0
    uint8 bReserved15 : 7;
    uint8 bReserved16 : 8;
    uint8 bReserved17 : 8;
    uint8 bReserved18 : 3;
    uint8 bOemOpenBrackets : 1;   // VK_OEM_4, 0xDB
    uint8 bOemPipe : 1;           // VK_OEM_5, 0xDC
    uint8 bOemCloseBrackets : 1;  // VK_OEM_6, 0xDD
    uint8 bOemQuotes : 1;         // VK_OEM_7, 0xDE
    uint8 bOem8 : 1;              // VK_OEM_8, 0xDF
    uint8 bReserved19 : 2;
    uint8 bOemBackslash : 1;      // VK_OEM_102, 0xE2
    uint8 bReserved20 : 2;
    uint8 bProcessKey : 1;        // VK_PROCESSKEY, 0xE5
    uint8 bReserved21 : 2;
    uint8 bReserved22 : 8;
    uint8 bReserved23 : 2;
    uint8 bOemCopy : 1;           // 0XF2
    uint8 bOemAuto : 1;           // 0xF3
    uint8 bOemEnlW : 1;           // 0xF4
    uint8 bReserved24 : 1;
    uint8 bAttn : 1;              // VK_ATTN, 0xF6
    uint8 bCrsel : 1;             // VK_CRSEL, 0xF7
    uint8 bExsel : 1;             // VK_EXSEL, 0xF8
    uint8 bEraseEof : 1;          // VK_EREOF, 0xF9
    uint8 bPlay : 1;              // VK_PLAY, 0xFA
    uint8 bZoom : 1;              // VK_ZOOM, 0xFB
    uint8 bReserved25 : 1;
    uint8 bPa1 : 1;               // VK_PA1, 0xFD
    uint8 bOemClear : 1;          // VK_OEM_CLEAR, 0xFE
    uint8 bReserved26 : 1;

    inline bool IsKeyDown(EKey key) const noexcept
    {
        return IsKeyDown((int32)key);
    }

    inline bool IsKeyDown(int32 key) const noexcept
    {
        if (key <= 0xfe)
        {
            auto ptr = reinterpret_cast<const uint32*>(this);
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
            auto ptr = reinterpret_cast<const uint32*>(this);
            unsigned int bf = 1u << (key & 0x1f);
            return (ptr[(key >> 5)] & bf) == 0;
        }
        return false;
    }
};

struct MouseState
{
    uint8 bLeftButton : 1;
    uint8 bMiddleButton : 1;
    uint8 bRightButton : 1;
    uint8 bXButton1 : 1;
    uint8 bXButton2 : 1;
    int32 X;
    int32 Y;
    int32 ScrollWheelValue;
    EMousePositionMode Mode;
};