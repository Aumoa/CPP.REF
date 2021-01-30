// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "Key.h"

struct KeyboardState
{
private:
	std::vector<bool> states;

public:
	KeyboardState();
	KeyboardState(const KeyboardState& rh);
	KeyboardState(KeyboardState&& rh);
	KeyboardState(std::vector<bool>&& inKeyStates);
	~KeyboardState();

	bool HasKeyDown(EKey inKey) const;
	bool HasKeyUp(EKey inKey) const;

	KeyboardState& operator =(const KeyboardState& rh);
	KeyboardState& operator =(KeyboardState&& rh);
};

struct KeyboardCompare
{
private:
	std::vector<bool> keyDown;
	std::vector<bool> keyUp;

public:
	KeyboardCompare();
	~KeyboardCompare();

	void Compare(const KeyboardState& lh, const KeyboardState& rh);

	bool IsKeyDown(EKey inKey) const;
	bool IsKeyUp(EKey inKey) const;
};

class PlatformInput : virtual public Object
{
public:
	static KeyboardState GetKeyboardState();
};