// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "EngineSubsystems/GameInputSystem.h"
#include "PlatformMisc/IPlatformMouse.h"
#include "PlatformMisc/IPlatformKeyboard.h"

SMouseStateTracker SGameInputSystem::_mouseTracker;
SKeyboardTracker SGameInputSystem::_keyboardTracker;
std::optional<int32> SGameInputSystem::_lastMouseX;
std::optional<int32> SGameInputSystem::_lastMouseY;

SGameInputSystem::SGameInputSystem() : Super()
{
}

SGameInputSystem::~SGameInputSystem()
{
}

void SGameInputSystem::Tick(std::chrono::duration<float> elapsedTime)
{
	// Update tracking state.
	IPlatformMouse& wMouse = IPlatformMouse::Get();
	IPlatformKeyboard& wKeyboard = IPlatformKeyboard::Get();

	MouseState mState = wMouse.GetState();
	KeyboardState kState = wKeyboard.GetState();

	_mouseTracker.Update(mState);
	_keyboardTracker.Update(kState);

	Vector2N mLocation = Vector2N(mState.X, mState.Y);

	// Invoke mouse dispatcher.
	uint32 nMouse = ((_mouseTracker.GetNumBits() - 1) / 32) + 1;
	for (uint32 i = 0; i < nMouse; ++i)
	{
		const uint32& pressedBits = _mouseTracker.GetPressedBitsByIndex(i);
		const uint32& releasedBits = _mouseTracker.GetReleasedBitsByIndex(i);

		uint32 n = 1;
		for (uint32 j = 0; j < 32; ++j)
		{
			uint32 idx = i * 32 + j;
			if (idx >= _mouseTracker.GetNumBits())
			{
				continue;
			}

			const bool bPressed = (pressedBits & n) != 0;
			const bool bReleased = (releasedBits & n) != 0;

			if (bPressed)
			{
				Mouse.Invoke(mLocation, (EMouseButton)idx, EMouseButtonEvent::Pressed);
			}
			if (bReleased)
			{
				Mouse.Invoke(mLocation, (EMouseButton)idx, EMouseButtonEvent::Released);
			}

			n <<= 1;
		}
	}

	// Invoke keyboard dispatcher.
	uint32 nKeyboard = ((_keyboardTracker.GetNumBits() - 1) / 32) + 1;
	for (uint32 i = 0; i < nKeyboard; ++i)
	{
		const uint32& pressedBits = _keyboardTracker.GetPressedBitsByIndex(i);
		const uint32& releasedBits = _keyboardTracker.GetReleasedBitsByIndex(i);

		uint32 n = 1;
		for (uint32 j = 0; j < 32; ++j)
		{
			uint32 idx = i * 32 + j;
			if (idx >= _keyboardTracker.GetNumBits())
			{
				continue;
			}

			const bool bPressed = (pressedBits & n) != 0;
			const bool bReleased = (releasedBits & n) != 0;

			if (bPressed)
			{
				Keyboard.Invoke((EKey)idx, EKeyboardEvent::Pressed);
			}
			if (bReleased)
			{
				Keyboard.Invoke((EKey)idx, EKeyboardEvent::Released);
			}

			n <<= 1;
		}
	}

	// Invoke mouse move event dispatcher.
	MouseMoveData data;
	data.CurX = mState.X;
	data.CurY = mState.Y;
	if (_lastMouseX.has_value())
	{
		data.DeltaX = mState.X - *_lastMouseX;
	}
	if (_lastMouseY.has_value())
	{
		data.DeltaY = mState.Y - *_lastMouseY;
	}
	_lastMouseX = mState.X;
	_lastMouseY = mState.Y;
	MouseMove.Invoke(data);
}