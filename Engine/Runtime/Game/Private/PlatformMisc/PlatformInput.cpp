// Copyright 2020 Aumoa.lib. All right reserved.

#include "PlatformMisc/PlatformInput.h"

#include "WindowsMinimal.h"
#include "Application.h"
#include "Logging/LogMacros.h"

using namespace std;

KeyboardState::KeyboardState()
{

}

KeyboardState::KeyboardState(const KeyboardState& rh)
	: states(rh.states)
{

}

KeyboardState::KeyboardState(KeyboardState&& rh)
	: states(std::move(rh.states))
{

}

KeyboardState::KeyboardState(vector<bool>&& inKeyStates)
	: states(move(inKeyStates))
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
	if (keyIdx >= states.size())
	{
		SE_LOG(LogPlatform, Error, L"Key index {0} is not supported on this platform.", keyIdx);
		return false;
	}

	return states[keyIdx];
}

bool KeyboardState::HasKeyUp(EKey inKey) const
{
	if (states.size() == 0)
	{
		return false;
	}

	size_t keyIdx = (size_t)inKey;
	if (keyIdx >= states.size())
	{
		SE_LOG(LogPlatform, Error, L"Key index {0} is not supported on this platform.", keyIdx);
		return false;
	}

	return !states[keyIdx];
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
	for (size_t i = 0; i < 256; ++i)
	{
		auto keyIdx = (EKey)i;

		bool lhd = lh.HasKeyDown(keyIdx);
		bool rhd = rh.HasKeyDown(keyIdx);

		if (!lhd && rhd)
		{
			keyDown[i] = true;
		}
		else
		{
			keyDown[i] = false;
		}

		if (lhd && !rhd)
		{
			keyUp[i] = true;
		}
		else
		{
			keyUp[i] = false;
		}
	}
}

bool KeyboardCompare::IsKeyDown(EKey inKey) const
{
	size_t keyIdx = (size_t)inKey;
	if (keyIdx >= keyDown.size())
	{
		SE_LOG(LogPlatform, Error, L"Key index {0} is not supported on this platform.", keyIdx);
		return false;
	}

	return keyDown[keyIdx];
}

bool KeyboardCompare::IsKeyUp(EKey inKey) const
{
	size_t keyIdx = (size_t)inKey;
	if (keyIdx >= keyUp.size())
	{
		SE_LOG(LogPlatform, Error, L"Key index {0} is not supported on this platform.", keyIdx);
		return false;
	}

	return keyUp[keyIdx];
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

	HWND hWnd = GApplication.GetCoreHwnd();
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

KeyboardState PlatformInput::GetKeyboardState()
{
	vector<bool> states;
	states.resize(256, false);

	if (!IsInputActive())
	{
		return states;
	}

	uint8 keyBytes[256];
	if (!::GetKeyboardState(keyBytes))
	{
		SE_LOG(LogPlatform, Error, L"Cannot query keyboard states.");
		return KeyboardState();
	}

	for (size_t i = 0; i < 256; ++i)
	{
		states[i] = (keyBytes[i] & 0x80) != 0;
	}

	return states;
}

CursorState PlatformInput::GetCursorState()
{
	CURSORINFO cinfo = { };
	cinfo.cbSize = sizeof(cinfo);
	if (!::GetCursorInfo(&cinfo))
	{
		SE_LOG(LogPlatform, Error, L"Cannot query cursor states.");
		return CursorState();
	}

	auto cursorState = CursorState((int32)cinfo.ptScreenPos.x, (int32)cinfo.ptScreenPos.y, cinfo.flags != 0);
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
	return GetActiveWindow() == GApplication.GetCoreHwnd();
}

void PlatformInput::SetCursorToHwndCenter()
{
	int32 x, y;
	GetCenterCursor(x, y);
	SetCursorPos(x, y);
}

void PlatformInput::GetCenterCursor(int32& x, int32& y)
{
	HWND hWnd = GApplication.GetCoreHwnd();
	RECT rc;
	GetClientRect(hWnd, &rc);

	POINT pt;
	pt.x = (rc.right - rc.left) / 2;
	pt.y = (rc.bottom - rc.top) / 2;

	ClientToScreen(hWnd, &pt);
	x = pt.x;
	y = pt.y;
}