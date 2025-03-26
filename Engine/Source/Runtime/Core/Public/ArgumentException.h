// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"

namespace Ayla
{
	class CORE_API ArgumentException : public Exception
	{
	public:
		ArgumentException(String InArgumentName)
			: Exception(String::Format(TEXT("Invalid argument {} detected."), InArgumentName))
		{
		}
	};
}