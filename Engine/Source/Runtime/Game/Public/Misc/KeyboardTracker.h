// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameStructures.h"
#include "BitfieldInputChangeTracker.h"

class KeyboardTracker : virtual public Object
{
	GENERATED_BODY(KeyboardTracker)

private:
	BitfieldInputChangeTracker<KeyboardState> _tracker;

public:
	inline KeyboardTracker()
	{
	}

	inline void Update(const KeyboardState& newState)
	{
		_tracker.Update(newState);
	}

	inline void Reset()
	{
		_tracker.Reset();
	}

	inline const KeyboardState& GetLast() const
	{
		return _tracker.GetLast();
	}

	inline bool IsKeyPressed(EKey key) const
	{
		return _tracker.IsPressedByBitIndex((uint32)key);
	}

	inline const uint32& GetPressedBitsByIndex(uint32 bitIndex) const
	{
		return _tracker.GetPressedBitIndex(bitIndex);
	}

	inline bool IsKeyReleased(EKey key) const
	{
		return _tracker.IsReleasedByBitIndex((uint32)key);
	}

	inline const uint32& GetReleasedBitsByIndex(uint32 bitIndex) const
	{
		return _tracker.GetReleasedBitIndex(bitIndex);
	}

	inline uint32 GetNumBits() const
	{
		return (uint32)EKey::Max;
	}
};