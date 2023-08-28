// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "System/Exception.h"

class CORE_API NullReferenceException : public Exception
{
public:
	NullReferenceException()
		: Exception(TEXT("Object reference not set to an instance of an object."))
	{
	}
};