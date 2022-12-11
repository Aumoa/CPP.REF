// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/AccessViolationException.h"

AccessViolationException::AccessViolationException(EAccessMode access, int64 location)
	: Exception(FormatMessage(access, location))
{
}

String AccessViolationException::FormatMessage(EAccessMode access, int64 location)
{
	String accessStr;
	switch (access)
	{
	case EAccessMode::Read:
		accessStr = TEXT("Reading");
		break;
	case EAccessMode::Write:
		accessStr = TEXT("Writing");
		break;
	default:
		accessStr = TEXT("Accessing");
		break;
	}

	return String::Format(TEXT("Access Violation: {} [{}] pointer."), accessStr, (void*)location);
}