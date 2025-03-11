// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "System/Exception.h"

class CORE_API ArgumentNullException : public Exception
{
public:
	ArgumentNullException(String InArgumentName)
		: Exception(String::Format(TEXT("Argument {} cannot be nullptr."), InArgumentName))
	{
	}
};