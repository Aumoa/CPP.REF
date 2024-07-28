// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "System/LanguageSupportMacros.h"

export module Core:AccessViolationException;

export import :Exception;
export import :MemoryAccessMode;

export class CORE_API AccessViolationException : public Exception
{
public:
	AccessViolationException(EMemoryAccessMode InAccessMode, int64 MemoryLocation)
		: Exception(FormatMessage(InAccessMode, MemoryLocation))
	{
	}

private:
	static String FormatMessage(EMemoryAccessMode InAccessMode, int64 MemoryLocation)
	{
		String AccessStr;
		switch (InAccessMode)
		{
		case EMemoryAccessMode::Read:
			AccessStr = TEXT("Reading");
			break;
		case EMemoryAccessMode::Write:
			AccessStr = TEXT("Writing");
			break;
		default:
			AccessStr = TEXT("Accessing");
			break;
		}

		return String::Format(TEXT("Access Violation: {} [{}] pointer."), AccessStr, MemoryLocation);
	}
};