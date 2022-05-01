// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Reflection/Enum.h"

namespace libty::inline Core
{
	SENUM(CORESOBJECT_API, EServiceType, int32,
		HostedService,,
		Singleton,,
		Scoped,
	);
}