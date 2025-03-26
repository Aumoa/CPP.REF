// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "ArgumentOutOfRangeException.h"

namespace Ayla
{
	ArgumentOutOfRangeException::ArgumentOutOfRangeException()
		: Exception(TEXT("Argument is out of range."))
	{
	}
}