// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "EngineSubsystems/GameInputSystem.h"
#include "PlatformMisc/WinMouse.h"
#include "PlatformMisc/WinKeyboard.h"

MouseStateTracker GameInputSystem::_mouseTracker;
KeyboardTracker GameInputSystem::_keyboardTracker;
std::optional<int32> GameInputSystem::_lastMouseX;
std::optional<int32> GameInputSystem::_lastMouseY;

GameInputSystem::GameInputSystem() : Super()
{
}

GameInputSystem::~GameInputSystem()
{
}

void GameInputSystem::Tick(std::chrono::duration<float> elapsedTime)
{
	// Update tracking state.
	WinMouse& wMouse = WinMouse::Get();
	WinKeyboard& wKeyboard = WinKeyboard::Get();

	_mouseTracker.Update(wMouse.GetState());
	_keyboardTracker.Update(wKeyboard.GetState());

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
				Mouse.Invoke((EMouseButton)idx, EMouseButtonEvent::Pressed);
			}
			if (bReleased)
			{
				Mouse.Invoke((EMouseButton)idx, EMouseButtonEvent::Released);
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
	MouseState mState = wMouse.GetState();
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