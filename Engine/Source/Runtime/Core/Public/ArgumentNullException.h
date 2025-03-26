// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"

namespace Ayla
{
	class CORE_API ArgumentNullException : public Exception
	{
	public:
		ArgumentNullException(String InArgumentName)
			: Exception(String::Format(TEXT("Argument {} cannot be nullptr."), InArgumentName))
		{
		}
	};
}