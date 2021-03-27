// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Input/Mouse.h"

MouseStateTracker::MouseStateTracker() : Super()
{

}

MouseStateTracker::~MouseStateTracker()
{

}

void MouseStateTracker::Update(const MouseState& inNewState)
{
	tracker.Update(inNewState);
}

void MouseStateTracker::Reset()
{
	tracker.Reset();
}

bool MouseStateTracker::IsButtonPressed(EMouseButton inButton) const
{
	return tracker.IsPressedByBitIndex((uint32)inButton);
}

bool MouseStateTracker::IsButtonReleased(EMouseButton inButton) const
{
	return tracker.IsReleasedByBitIndex((uint32)inButton);
}