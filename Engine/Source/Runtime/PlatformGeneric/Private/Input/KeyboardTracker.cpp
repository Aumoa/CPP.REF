// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Input/KeyboardTracker.h"

using namespace ::libty;

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
	return Tracker.IsPressedByBitIndex((int32)Key);
}

int32 SKeyboardTracker::GetPressedBitsByIndex(int32 BitIndex)
{
	return Tracker.GetPressedBitIndex(BitIndex);
}

bool SKeyboardTracker::IsKeyReleased(EKey Key)
{
	return Tracker.IsReleasedByBitIndex((int32)Key);
}

int32 SKeyboardTracker::GetReleasedBitsByIndex(int32 BitIndex)
{
	return Tracker.GetReleasedBitIndex(BitIndex);
}

int32 SKeyboardTracker::GetNumBits()
{
	return (int32)EKey::Max;
}