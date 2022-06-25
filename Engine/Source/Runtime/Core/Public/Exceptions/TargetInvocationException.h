// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"

class TargetInvocationException : public Exception
{
public:
	TargetInvocationException(const String& message)
		: Exception(message)
	{
	}
};