// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"

namespace Ayla
{
	class CORE_API InvalidCastException : public Exception
	{
	public:
		InvalidCastException(String InMessage, std::exception_ptr InInnerException)
			: Exception(InMessage, InInnerException)
		{
		}
	};
}