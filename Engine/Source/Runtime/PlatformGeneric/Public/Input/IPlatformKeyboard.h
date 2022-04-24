// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "KeyboardState.h"

namespace libty::inline PlatformGeneric
{
	struct PLATFORMGENERIC_API IPlatformKeyboard : virtual public SObject
	{
		GENERATED_INTERFACE_BODY(IPlatformKeyboard);

	public:
		virtual KeyboardState GetState() = 0;
		virtual void Reset() = 0;
		virtual bool IsConnected() = 0;

		DECLARE_MULTICAST_DELEGATE(KeyboardDelegate, EKey);
		KeyboardDelegate KeyPressed;
		KeyboardDelegate KeyReleased;
	};
}