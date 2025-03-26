// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "RealTypes.h"
#include "String_.h"
#include "Platform/PlatformLocalization.h"
#include <cstdlib>

namespace Ayla
{
	template<>
	String TRealType<float>::ToString() const
	{
		return String::Format(TEXT("{}"), Value);
	}

	template<>
	String TRealType<double>::ToString() const
	{
		return String::Format(TEXT("{}"), Value);
	}
}