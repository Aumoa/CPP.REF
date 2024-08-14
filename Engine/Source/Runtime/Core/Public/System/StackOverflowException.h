// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "System/Exception.h"

class CORE_API StackOverflowException : public Exception
{
public:
	StackOverflowException()
		: Exception(TEXT("The requested operation caused a stack overflow."))
	{
	}
};