// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "KeyboardState.h"

struct GAME_API IPlatformKeyboard : implements SObject
{
	virtual KeyboardState GetState() const = 0;
	virtual void Reset() = 0;
	virtual bool IsConnected() const = 0;

	static IPlatformKeyboard& Get();
};