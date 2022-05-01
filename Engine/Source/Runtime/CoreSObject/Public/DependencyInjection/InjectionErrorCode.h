// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Reflection/Enum.h"
#include "ValueType.h"

namespace libty::inline Core
{
	SENUM(CORESOBJECT_API, EInjectionErrorCode, int32,
		InjectFunctionNotFound, -1,
		InjectInnerException, -2,
		NotSupportedType, -3,
		ServiceNotFound, -4,
		ServiceNotConstructible, -5
	);
}