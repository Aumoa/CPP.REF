// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Input/MouseStateTracker.h"

using namespace ::libty;

SMouseStateTracker::SMouseStateTracker() : Super()
{
}

void SMouseStateTracker::Update(const MouseState& InNewState)
{
	Tracker.Update(InNewState);
}

void SMouseStateTracker::Reset()
{
	Tracker.Reset();
}

const MouseState& SMouseStateTracker::GetLast()
{
	return Tracker.GetLast();
}

bool SMouseStateTracker::IsButtonPressed(EMouseButton Button)
{
	return Tracker.IsPressedByBitIndex((int32)Button);
}

int32 SMouseStateTracker::GetPressedBitsByIndex(int32 bitIndex)
{
	return Tracker.GetPressedBitIndex(bitIndex);
}

bool SMouseStateTracker::IsButtonReleased(EMouseButton inButton)
{
	return Tracker.IsReleasedByBitIndex((int32)inButton);
}

int32 SMouseStateTracker::GetReleasedBitsByIndex(int32 bitIndex)
{
	return Tracker.GetReleasedBitIndex(bitIndex);
}

int32 SMouseStateTracker::GetNumBits()
{
	return 5;
}