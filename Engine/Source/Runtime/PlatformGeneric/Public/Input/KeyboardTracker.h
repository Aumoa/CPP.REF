// Copyright 2020-2022 Aumoa.lib. All right reserved.

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
	int32 GetPressedBitsByIndex(int32 BitIndex);
	bool IsKeyReleased(EKey Key);
	int32 GetReleasedBitsByIndex(int32 BitIndex);
	int32 GetNumBits();
};