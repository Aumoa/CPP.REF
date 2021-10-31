// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Input/MouseStateTracker.h"

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
	return Tracker.IsPressedByBitIndex((uint32)Button);
}

const uint32& SMouseStateTracker::GetPressedBitsByIndex(uint32 bitIndex)
{
	return Tracker.GetPressedBitIndex(bitIndex);
}

bool SMouseStateTracker::IsButtonReleased(EMouseButton inButton)
{
	return Tracker.IsReleasedByBitIndex((uint32)inButton);
}

const uint32& SMouseStateTracker::GetReleasedBitsByIndex(uint32 bitIndex)
{
	return Tracker.GetReleasedBitIndex(bitIndex);
}

uint32 SMouseStateTracker::GetNumBits()
{
	return 5;
}