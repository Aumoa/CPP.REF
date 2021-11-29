// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "BitfieldInputChangeTracker.h"
#include "KeyboardState.h"
#include "MouseState.h"

class PLATFORMGENERIC_API SKeyboardTracker : implements SObject
{
	GENERATED_BODY(SKeyboardTracker)

private:
	BitfieldInputChangeTracker<KeyboardState> Tracker;

public:
	SKeyboardTracker();

	void Update(const KeyboardState& InNewState);
	void Reset();
	const KeyboardState& GetLast();
	bool IsKeyPressed(EKey Key);
	const uint32& GetPressedBitsByIndex(uint32 BitIndex);
	bool IsKeyReleased(EKey Key);
	const uint32& GetReleasedBitsByIndex(uint32 BitIndex);
	uint32 GetNumBits();
};