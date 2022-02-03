// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "EngineSubsystems/GameInputSystem.h"
#include "Input/IPlatformKeyboard.h"
#include "Input/IPlatformMouse.h"
#include "IApplicationInterface.h"

GENERATE_BODY(SGameInputSystem);

SGameInputSystem::SGameInputSystem() : Super()
{
	MouseTracker = gcnew SMouseStateTracker();
	KeyboardTracker = gcnew SKeyboardTracker();
}

void SGameInputSystem::Tick(float InDeltaTime)
{
	// Update tracking state.
	auto& App = IApplicationInterface::Get();
	IPlatformMouse& wMouse = App.GetPlatformMouse();
	IPlatformKeyboard& wKeyboard = App.GetPlatformKeyboard();

	MouseState mState = wMouse.GetState();
	KeyboardState kState = wKeyboard.GetState();

	MouseTracker->Update(mState);
	KeyboardTracker->Update(kState);

	Vector2N mLocation = Vector2N(mState.X, mState.Y);

	// Invoke mouse dispatcher.
	int32 nMouse = ((MouseTracker->GetNumBits() - 1) / 32) + 1;
	for (int32 i = 0; i < nMouse; ++i)
	{
		const int32& PressedBits = MouseTracker->GetPressedBitsByIndex(i);
		const int32& ReleasedBits = MouseTracker->GetReleasedBitsByIndex(i);

		int32 N = 1;
		for (int32 j = 0; j < 32; ++j)
		{
			int32 Index = i * 32 + j;
			if (Index >= MouseTracker->GetNumBits())
			{
				continue;
			}

			const bool bPressed = (PressedBits & N) != 0;
			const bool bReleased = (ReleasedBits & N) != 0;

			if (bPressed)
			{
				Mouse.Broadcast(mLocation, (EMouseButton)Index, EMouseButtonEvent::Pressed);
			}
			if (bReleased)
			{
				Mouse.Broadcast(mLocation, (EMouseButton)Index, EMouseButtonEvent::Released);
			}

			N <<= 1;
		}
	}

	// Invoke keyboard dispatcher.
	int32 nKeyboard = ((KeyboardTracker->GetNumBits() - 1) / 32) + 1;
	for (int32 i = 0; i < nKeyboard; ++i)
	{
		const int32& PressedBits = KeyboardTracker->GetPressedBitsByIndex(i);
		const int32& ReleasedBits = KeyboardTracker->GetReleasedBitsByIndex(i);

		int32 N = 1;
		for (int32 j = 0; j < 32; ++j)
		{
			int32 Index = i * 32 + j;
			if (Index >= KeyboardTracker->GetNumBits())
			{
				continue;
			}

			const bool bPressed = (PressedBits & N) != 0;
			const bool bReleased = (ReleasedBits & N) != 0;

			if (bPressed)
			{
				Keyboard.Broadcast((EKey)Index, EKeyboardEvent::Pressed);
			}
			if (bReleased)
			{
				Keyboard.Broadcast((EKey)Index, EKeyboardEvent::Released);
			}

			N <<= 1;
		}
	}

	// Invoke mouse move event dispatcher.
	MouseMoveData Data;
	Data.CurX = mState.X;
	Data.CurY = mState.Y;
	if (LastMouseX.has_value())
	{
		Data.DeltaX = mState.X - *LastMouseX;
	}
	if (LastMouseY.has_value())
	{
		Data.DeltaY = mState.Y - *LastMouseY;
	}
	LastMouseX = mState.X;
	LastMouseY = mState.Y;
	MouseMove.Broadcast(Data);
}