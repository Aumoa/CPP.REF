// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "BitfieldInputChangeTracker.h"
#include "Input/IPlatformMouse.h"

class PLATFORMGENERIC_API SMouseStateTracker : implements SObject
{
	GENERATED_BODY(SMouseStateTracker)

private:
	BitfieldInputChangeTracker<MouseState> Tracker;

public:
	SMouseStateTracker();

	void Update(const MouseState& InNewState);
	void Reset();
	const MouseState& GetLast();
	bool IsButtonPressed(EMouseButton inButton);
	int32 GetPressedBitsByIndex(int32 bitIndex);
	bool IsButtonReleased(EMouseButton inButton);
	int32 GetReleasedBitsByIndex(int32 bitIndex);
	int32 GetNumBits();
};