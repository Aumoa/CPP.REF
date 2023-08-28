// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "System/Exception.h"

class CORE_API NotSupportedException : public Exception
{
public:
	NotSupportedException(std::exception_ptr InInnerException = nullptr)
		: Exception(TEXT("The specified function or operation is not supported."), InInnerException)
	{
	}
};