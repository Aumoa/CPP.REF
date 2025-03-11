// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "System/Exception.h"

class CORE_API InvalidCastException : public Exception
{
public:
	InvalidCastException(String InMessage, std::exception_ptr InInnerException)
		: Exception(InMessage, InInnerException)
	{
	}
};