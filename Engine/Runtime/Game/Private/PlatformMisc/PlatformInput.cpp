// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "PlatformMisc/PlatformInput.h"

#include "Windows/WindowsMinimal.h"
#include "Windows/Application.h"
#include "Windows/CoreWindow.h"
#include "Logging/LogMacros.h"
#include "Diagnostics/ScopedCycleCounter.h"

DEFINE_STATS_GROUP(PlatformInput);

using namespace std;

KeyboardState::KeyboardState()
{

}

KeyboardState::KeyboardState(const KeyboardState& rh)
	: states(rh.states)
{

}

KeyboardState::KeyboardState(KeyboardState&& rh) noexcept
	: states(std::move(rh.states))
{

}

KeyboardState::KeyboardState(const vector<char>& inKeyStates)
	: states(inKeyStates)
{

}

KeyboardState::~KeyboardState()
{

}

bool KeyboardState::HasKeyDown(EKey inKey) const
{
	if (states.size() == 0)
	{
		return false;
	}

	size_t keyIdx = (size_t)inKey;
	if (keyIdx >= states.size() * 8)
	{
		SE_LOG(LogPlatform, Error, L"Key index {0} is not supported on this platform.", keyIdx);
		return false;
	}

	size_t fastA = keyIdx / 8;
	size_t fastB = keyIdx % 8;

	return ((states[keyIdx / 8] >> fastB) & 1) == 1;
}

bool KeyboardState::HasKeyUp(EKey inKey) const
{
	return !HasKeyDown(inKey);
}

KeyboardState& KeyboardState::operator =(const KeyboardState& rh)
{
	states = rh.states;
	return *this;
}

KeyboardState& KeyboardState::operator =(KeyboardState&& rh)
{
	states = move(rh.states);
	return *this;
}

KeyboardCompare::KeyboardCompare()
{
	keyDown.resize(256, false);
	keyUp.resize(256, false);
}

KeyboardCompare::~KeyboardCompare()
{

}

void KeyboardCompare::Compare(const KeyboardState& lh, const KeyboardState& rh)
{
	if (lh.states.empty() || rh.states.empty())
	{
		return;
	}

	keyDown.resize(32);
	keyUp.resize(32);

	for (size_t i = 0; i < 32; ++i)
	{
		const char& lhc = lh.states[i];
		const char& rhc = rh.states[i];

		keyDown[i] = rhc & (lhc ^ rhc);
		keyUp[i] = lhc & (lhc ^ rhc);
	}
}

bool KeyboardCompare::IsKeyDown(EKey inKey) const
{
	size_t keyIdx = (size_t)inKey;
	if (keyIdx >= keyDown.size() * 8)
	{
		SE_LOG(LogPlatform, Error, L"Key index {0} is not supported on this platform.", keyIdx);
		return false;
	}

	size_t fastA = keyIdx / 8;
	size_t fastB = keyIdx % 8;

	return ((keyDown[keyIdx / 8] >> fastB) & 1) == 1;
}

bool KeyboardCompare::IsKeyUp(EKey inKey) const
{
	size_t keyIdx = (size_t)inKey;
	if (keyIdx >= keyUp.size() * 8)
	{
		SE_LOG(LogPlatform, Error, L"Key index {0} is not supported on this platform.", keyIdx);
		return false;
	}

	size_t fastA = keyIdx / 8;
	size_t fastB = keyIdx % 8;

	return ((keyUp[keyIdx / 8] >> fastB) & 1) == 1;
}

CursorState::CursorState()
	: cursorX(0)
	, cursorY(0)
	, bCursorVisible(false)
	, bInitialized(false)
{

}

CursorState::CursorState(int32 cursorX, int32 cursorY, bool bCursorVisible)
	: cursorX(cursorX)
	, cursorY(cursorY)
	, bInitialized(PlatformInput::IsInputActive())
	, bCursorVisible(bCursorVisible)
{

}

int32 CursorState::GetCursorX() const
{
	return cursorX;
}

int32 CursorState::GetCursorY() const
{
	return cursorY;
}

bool CursorState::IsCursorVisible() const
{
	return bCursorVisible;
}

bool CursorState::IsInitialized() const
{
	return bInitialized;
}

CursorCompare::CursorCompare() : cursorDeltaX(0), cursorDeltaY(0), dpiDeltaX(0), dpiDeltaY(0)
{

}

void CursorCompare::Compare(const CursorState& lh, const CursorState& rh)
{
	if (!lh.IsInitialized() || !rh.IsInitialized())
	{
		return;
	}

	int32 lhx, lhy;
	if (lh.IsCursorVisible() == false)
	{
		PlatformInput::GetCenterCursor(lhx, lhy);
	}
	else
	{
		lhx = lh.GetCursorX();
		lhy = lh.GetCursorY();
	}

	HWND hWnd = GApplication.MainWindow->HWnd;
	uint32 dpi = GetDpiForWindow(hWnd);

	cursorDeltaX = rh.GetCursorX() - lhx;
	cursorDeltaY = rh.GetCursorY() - lhy;

	dpiDeltaX = cursorDeltaX / (float)dpi;
	dpiDeltaY = cursorDeltaY / (float)dpi;
}

int32 CursorCompare::GetDeltaX() const
{
	return cursorDeltaX;
}

int32 CursorCompare::GetDeltaY() const
{
	return cursorDeltaY;
}

float CursorCompare::GetDeltaXByDpi() const
{
	return dpiDeltaX;
}

float CursorCompare::GetDeltaYByDpi() const
{
	return dpiDeltaY;
}

bool PlatformInput::bCaptureCursor = false;
vector<char> PlatformInput::bKeyboardStates(32);

int32 PlatformInput::cursorX = 0;
int32 PlatformInput::cursorY = 0;
bool PlatformInput::bCursorVisible = false;

void PlatformInput::Tick()
{
	uint8 keyboardStates[256];
	if (!::GetKeyboardState(keyboardStates))
	{
		SE_LOG(LogPlatform, Error, L"Cannot query keyboard states.");
		return;
	}

	for (size_t i = 0; i < 32; ++i)
	{
		char& c = bKeyboardStates[i];
		c = 0;
		for (size_t j = 0; j < 8; ++j)
		{
			size_t k = i * 8 + j;
			bool b = keyboardStates[k] & 0x80;
			char n = (char)b << j;
			c = c | n;
		}
	}

	CURSORINFO cinfo = { };
	cinfo.cbSize = sizeof(cinfo);
	if (!::GetCursorInfo(&cinfo))
	{
		SE_LOG(LogPlatform, Error, L"Cannot query cursor states.");
		return;
	}

	cursorX = cinfo.ptScreenPos.x;
	cursorY = cinfo.ptScreenPos.y;
	bCursorVisible = cinfo.flags != 0;
}

KeyboardState PlatformInput::GetKeyboardState()
{
	static vector<char> empty(32);

	if (!IsInputActive())
	{
		return empty;
	}

	return bKeyboardStates;
}

CursorState PlatformInput::GetCursorState()
{
	CursorState cursorState(cursorX, cursorY, bCursorVisible);
	if (bCaptureCursor)
	{
		SetCursorToHwndCenter();
	}

	return cursorState;
}

void PlatformInput::SetCursorVisible(bool value)
{
	int32 displayCount = ::ShowCursor(value);
	bCaptureCursor = displayCount < 0;
}

bool PlatformInput::IsInputActive()
{
	// If application is not active, all input states are undefined.
	return GetActiveWindow() == GApplication.MainWindow->HWnd;
}

void PlatformInput::SetCursorToHwndCenter()
{
	int32 x, y;
	GetCenterCursor(x, y);
	SetCursorPos(x, y);
}

void PlatformInput::GetCenterCursor(int32& x, int32& y)
{
	HWND hWnd = GApplication.MainWindow->HWnd;
	RECT rc;
	GetClientRect(hWnd, &rc);

	POINT pt;
	pt.x = (rc.right - rc.left) / 2;
	pt.y = (rc.bottom - rc.top) / 2;

	ClientToScreen(hWnd, &pt);
	x = pt.x;
	y = pt.y;
}