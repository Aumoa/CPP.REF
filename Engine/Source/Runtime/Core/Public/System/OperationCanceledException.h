// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "System/Exception.h"

class CORE_API OperationCanceledException : public Exception
{
public:
	OperationCanceledException(std::exception_ptr InInnerException = nullptr, std::source_location Src = std::source_location::current())
		: Exception(TEXT("Operation was aborted."), InInnerException, Src)
	{
	}
};