// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "IMEEvent.h"

namespace libty::inline PlatformGeneric::inline Input
{
	struct PLATFORMGENERIC_API IPlatformIME : virtual public SObject
	{
		GENERATED_BODY(IPlatformIME);

		DECLARE_MULTICAST_DELEGATE(IMEDelegate, IMEEvent);
		IMEDelegate IME;
	};
}