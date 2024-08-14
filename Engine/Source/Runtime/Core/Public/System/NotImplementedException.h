// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "System/Exception.h"

class NotImplementedException : public Exception
{
public:
	NotImplementedException()
		: Exception(TEXT("The function to call does not implemented."))
	{
	}
};