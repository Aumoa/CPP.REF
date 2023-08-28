// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "System/Exception.h"

class CORE_API TaskCanceledException : public Exception
{
public:
	TaskCanceledException(std::exception_ptr InInnerException = nullptr, std::source_location Src = std::source_location::current())
		: Exception(TEXT("Task was aborted."), InInnerException, Src)
	{
	}
};