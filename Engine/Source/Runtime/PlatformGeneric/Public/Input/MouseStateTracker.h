// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
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
	const uint32& GetPressedBitsByIndex(uint32 bitIndex);
	bool IsButtonReleased(EMouseButton inButton);
	const uint32& GetReleasedBitsByIndex(uint32 bitIndex);
	uint32 GetNumBits();
};