// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"
#include "MemoryAccessMode.h"

namespace Ayla
{
	class CORE_API AccessViolationException : public Exception
	{
	public:
		AccessViolationException(MemoryAccessMode InAccessMode, int64 MemoryLocation)
			: Exception(FormatMessage(InAccessMode, MemoryLocation))
		{
		}

	private:
		static String FormatMessage(MemoryAccessMode InAccessMode, int64 MemoryLocation)
		{
			String AccessStr;
			switch (InAccessMode)
			{
			case MemoryAccessMode::Read:
				AccessStr = TEXT("Reading");
				break;
			case MemoryAccessMode::Write:
				AccessStr = TEXT("Writing");
				break;
			default:
				AccessStr = TEXT("Accessing");
				break;
			}

			return String::Format(TEXT("Access Violation: {} [{}] pointer."), AccessStr, MemoryLocation);
		}
	};
}