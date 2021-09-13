// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameStructures.h"
#include "BitfieldInputChangeTracker.h"

class SMouseStateTracker : virtual public SObject
{
	GENERATED_BODY(SMouseStateTracker)

private:
	BitfieldInputChangeTracker<MouseState> _tracker;

public:
	inline SMouseStateTracker()
	{
	}

	inline void Update(const MouseState& newState)
	{
		_tracker.Update(newState);
	}

	inline void Reset()
	{
		_tracker.Reset();
	}

	inline const MouseState& GetLast()
	{
		return _tracker.GetLast();
	}

	inline bool IsButtonPressed(EMouseButton inButton) const
	{
		return _tracker.IsPressedByBitIndex((uint32)inButton);
	}

	inline const uint32& GetPressedBitsByIndex(uint32 bitIndex) const
	{
		return _tracker.GetPressedBitIndex(bitIndex);
	}

	inline bool IsButtonReleased(EMouseButton inButton) const
	{
		return _tracker.IsReleasedByBitIndex((uint32)inButton);
	}

	inline const uint32& GetReleasedBitsByIndex(uint32 bitIndex) const
	{
		return _tracker.GetReleasedBitIndex(bitIndex);
	}

	inline uint32 GetNumBits() const
	{
		return 5;
	}
};