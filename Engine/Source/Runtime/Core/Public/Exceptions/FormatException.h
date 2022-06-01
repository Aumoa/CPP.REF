// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"

class FormatException : public Exception
{
public:
	FormatException(const String& message)
		: Exception(message)
	{
	}
};