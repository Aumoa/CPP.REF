// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "KeyboardState.h"

interface IPlatformKeyboard : implements SObject
{
	virtual KeyboardState GetState() = 0;
	virtual void Reset() = 0;
	virtual bool IsConnected() = 0;

	DECLARE_MULTICAST_DELEGATE(KeyboardDelegate, EKey);
	KeyboardDelegate KeyPressed;
	KeyboardDelegate KeyReleased;
};