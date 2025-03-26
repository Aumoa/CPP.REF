// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"

namespace Ayla
{
	class CORE_API StackOverflowException : public Exception
	{
	public:
		StackOverflowException()
			: Exception(TEXT("The requested operation caused a stack overflow."))
		{
		}
	};
}