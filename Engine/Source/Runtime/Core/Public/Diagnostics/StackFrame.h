// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "String_.h"

namespace Ayla
{
	struct CORE_API StackFrame
	{
		String Location;
		int32 Line = 0;
		String Module;
		String Description;
		int64 AddressOf = 0;
	};
}