// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"

namespace Ayla
{
	class CORE_API FormatException : public Exception
	{
	public:
		FormatException()
			: Exception(TEXT("Input string was not in a correct format."))
		{
		}

		FormatException(String message)
			: Exception(message)
		{
		}
	};
}