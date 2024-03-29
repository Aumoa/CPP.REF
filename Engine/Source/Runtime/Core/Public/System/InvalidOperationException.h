// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "System/Exception.h"

class CORE_API InvalidOperationException : public Exception
{
public:
	InvalidOperationException(const String& InMessage, std::exception_ptr InInnerException = nullptr, std::source_location Src = std::source_location::current())
		: Exception(InMessage, InInnerException, Src)
	{
	}

	InvalidOperationException(std::exception_ptr InInnerException = nullptr, std::source_location Src = std::source_location::current())
		: Exception(TEXT("Invalid operation detected."), InInnerException, Src)
	{
	}
};