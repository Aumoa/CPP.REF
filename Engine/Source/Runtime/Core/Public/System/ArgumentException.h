// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "System/Exception.h"

class CORE_API ArgumentException : public Exception
{
public:
	ArgumentException(String InArgumentName)
		: Exception(String::Format(TEXT("Invalid argument {} detected."), InArgumentName))
	{
	}
};