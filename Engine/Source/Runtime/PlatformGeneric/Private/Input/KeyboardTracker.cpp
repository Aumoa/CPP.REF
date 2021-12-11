// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Input/KeyboardTracker.h"

GENERATE_BODY(SKeyboardTracker);

SKeyboardTracker::SKeyboardTracker() : Super()
{
}

void SKeyboardTracker::Update(const KeyboardState& InNewState)
{
	Tracker.Update(InNewState);
}

void SKeyboardTracker::Reset()
{
	Tracker.Reset();
}

const KeyboardState& SKeyboardTracker::GetLast()
{
	return Tracker.GetLast();
}

bool SKeyboardTracker::IsKeyPressed(EKey Key)
{
	return Tracker.IsPressedByBitIndex((uint32)Key);
}

const uint32& SKeyboardTracker::GetPressedBitsByIndex(uint32 BitIndex)
{
	return Tracker.GetPressedBitIndex(BitIndex);
}

bool SKeyboardTracker::IsKeyReleased(EKey Key)
{
	return Tracker.IsReleasedByBitIndex((uint32)Key);
}

const uint32& SKeyboardTracker::GetReleasedBitsByIndex(uint32 BitIndex)
{
	return Tracker.GetReleasedBitIndex(BitIndex);
}

uint32 SKeyboardTracker::GetNumBits()
{
	return (uint32)EKey::Max;
}