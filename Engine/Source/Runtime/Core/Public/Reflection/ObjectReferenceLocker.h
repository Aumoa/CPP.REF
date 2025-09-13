// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "IntegralTypes.h"
#include <memory>

namespace Ayla
{
	class Object;

	struct CORE_API ObjectReferenceLocker
	{
		ssize_t Ref;
		int32 Flags;
	};
}