// Copyright 2020 Aumoa.lib. All right reserved.

#include "PlatformMisc/PlatformInput.h"

#include "WindowsMinimal.h"
#include "Logging/LogMacros.h"

using namespace std;

KeyboardState::KeyboardState()
{

}

KeyboardState::KeyboardState(const KeyboardState& rh)
	: states(rh.states)
{

}

KeyboardState::KeyboardState(KeyboardState&& rh)
	: states(std::move(rh.states))
{

}

KeyboardState::KeyboardState(vector<bool>&& inKeyStates)
	: states(move(inKeyStates))
{

}

KeyboardState::~KeyboardState()
{

}

bool KeyboardState::HasKeyDown(EKey inKey) const
{
	size_t keyIdx = (size_t)inKey;
	if (keyIdx >= states.size())
	{
		SE_LOG(LogPlatform, Error, L"Key index {0} is not supported on this platform.", keyIdx);
		return false;
	}

	return states[keyIdx];
}

bool KeyboardState::HasKeyUp(EKey inKey) const
{
	size_t keyIdx = (size_t)inKey;
	if (keyIdx >= states.size())
	{
		SE_LOG(LogPlatform, Error, L"Key index {0} is not supported on this platform.", keyIdx);
		return false;
	}

	return !states[keyIdx];
}

KeyboardState& KeyboardState::operator =(const KeyboardState& rh)
{
	states = rh.states;
	return *this;
}

KeyboardState& KeyboardState::operator =(KeyboardState&& rh)
{
	states = move(rh.states);
	return *this;
}

KeyboardCompare::KeyboardCompare()
{
	keyDown.resize(256, false);
	keyUp.resize(256, false);
}

KeyboardCompare::~KeyboardCompare()
{

}

void KeyboardCompare::Compare(const KeyboardState& lh, const KeyboardState& rh)
{
	for (size_t i = 0; i < 256; ++i)
	{
		auto keyIdx = (EKey)i;

		bool lhd = lh.HasKeyDown(keyIdx);
		bool rhd = rh.HasKeyDown(keyIdx);

		if (!lhd && rhd)
		{
			keyDown[i] = true;
		}
		else
		{
			keyDown[i] = false;
		}

		if (lhd && !rhd)
		{
			keyUp[i] = true;
		}
		else
		{
			keyUp[i] = false;
		}
	}
}

bool KeyboardCompare::IsKeyDown(EKey inKey) const
{
	size_t keyIdx = (size_t)inKey;
	if (keyIdx >= keyDown.size())
	{
		SE_LOG(LogPlatform, Error, L"Key index {0} is not supported on this platform.", keyIdx);
		return false;
	}

	return keyDown[keyIdx];
}

bool KeyboardCompare::IsKeyUp(EKey inKey) const
{
	size_t keyIdx = (size_t)inKey;
	if (keyIdx >= keyUp.size())
	{
		SE_LOG(LogPlatform, Error, L"Key index {0} is not supported on this platform.", keyIdx);
		return false;
	}

	return keyUp[keyIdx];
}

KeyboardState PlatformInput::GetKeyboardState()
{
	vector<bool> states;
	states.resize(256, false);

	uint8 keyBytes[256];
	if (!::GetKeyboardState(keyBytes))
	{
		SE_LOG(LogPlatform, Error, L"Cannot query keyboard states.");
		return states;
	}

	for (size_t i = 0; i < 256; ++i)
	{
		states[i] = (keyBytes[i] & 0x80) != 0;
	}

	return states;
}