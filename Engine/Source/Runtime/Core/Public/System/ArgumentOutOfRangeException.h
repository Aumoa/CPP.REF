// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "System/Exception.h"

class CORE_API ArgumentOutOfRangeException : public Exception
{
public:
	ArgumentOutOfRangeException()
		: Exception(TEXT("Argument is out of range."))
	{
	}
};