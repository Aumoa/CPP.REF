// Copyright 2020-2021 Aumoa.lib. All right reserved.

import std.core;
import SC.Runtime.Core;
import SC.Runtime.Game;

using namespace std;
using namespace std::chrono;

MouseStateTracker InputComponent::_mouseTracker;
KeyboardTracker InputComponent::_keyboardTracker;
set<InputComponent*> InputComponent::_inputComponents;

InputComponent::InputComponent() : Super()
{
}

void InputComponent::BeginPlay()
{
	Super::BeginPlay();
	_inputComponents.emplace(this);
}

void InputComponent::EndPlay()
{
	_inputComponents.erase(this);
	Super::EndPlay();
}

void InputComponent::StaticTick(duration<float> elapsedTime)
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
				BroadcastMouseEventForAllComponents(idx, EMouseButtonEvent::Pressed);
			}
			if (bReleased)
			{
				BroadcastMouseEventForAllComponents(idx, EMouseButtonEvent::Released);
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
				BroadcastKeyboardEventForAllComponents(idx, EKeyboardEvent::Pressed);
			}
			if (bReleased)
			{
				BroadcastKeyboardEventForAllComponents(idx, EKeyboardEvent::Released);
			}

			n <<= 1;
		}
	}
}

void InputComponent::BroadcastMouseEventForAllComponents(uint32 idx, EMouseButtonEvent buttonEvent)
{
	for (auto& cp : _inputComponents)
	{
		cp->MouseEvent.Invoke((EMouseButton)idx, buttonEvent);
	}
}

void InputComponent::BroadcastKeyboardEventForAllComponents(uint32 idx, EKeyboardEvent keyboardEvent)
{
	for (auto& cp : _inputComponents)
	{
		cp->KeyboardEvent.Invoke((EKey)idx, keyboardEvent);
	}
}