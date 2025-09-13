// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "AccessViolationException.h"

namespace Ayla
{
	AccessViolationException::AccessViolationException(String message)
		: Exception(message)
	{
	}

	AccessViolationException::AccessViolationException(MemoryAccessMode InAccessMode, int64 MemoryLocation)
		: Exception(FormatMessage(InAccessMode, MemoryLocation))
	{
	}
}