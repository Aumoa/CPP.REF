// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "KeyboardState.h"

struct IPlatformKeyboard : implements SObject
{
	GENERATED_INTERFACE_BODY(IPlatformKeyboard)

	virtual KeyboardState GetState() = 0;
	virtual void Reset() = 0;
	virtual bool IsConnected() = 0;

	DECLARE_MULTICAST_DELEGATE(KeyboardDelegate, EKey);
	KeyboardDelegate KeyPressed;
	KeyboardDelegate KeyReleased;
};