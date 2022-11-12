// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IO/KeyboardState.h"
#include "IPlatformKeyboard.gen.h"

SINTERFACE()
interface CORE_API IPlatformKeyboard : virtual public Object
{
	GENERATED_BODY()

    virtual KeyboardState GetState() = 0;
    virtual void Reset() = 0;
    virtual bool IsConnected() = 0;

    DECLARE_MULTICAST_DELEGATE(KeyEventDelegate, EKey);
    KeyEventDelegate KeyPressed;
    KeyEventDelegate KeyReleased;
};