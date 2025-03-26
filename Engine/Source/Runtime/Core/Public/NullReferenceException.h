// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"

namespace Ayla
{
	class CORE_API NullReferenceException : public Exception
	{
	public:
		NullReferenceException()
			: Exception(TEXT("Object reference not set to an instance of an object."))
		{
		}
	};
}