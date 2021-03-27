// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Input/Keyboard.h"

KeyStateTracker::KeyStateTracker() : Super()
{

}

KeyStateTracker::~KeyStateTracker()
{

}

void KeyStateTracker::Update(const KeyState& inNewState)
{
	tracker.Update(inNewState);
}

void KeyStateTracker::Reset()
{
	tracker.Reset();
}

bool KeyStateTracker::IsKeyPressed(EKey inKey) const
{
	return tracker.IsPressedByBitIndex((uint32)inKey);
}

bool KeyStateTracker::IsKeyReleased(EKey inKey) const
{
	return tracker.IsReleasedByBitIndex((uint32)inKey);
}