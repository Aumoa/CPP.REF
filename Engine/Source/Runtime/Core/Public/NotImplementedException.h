// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"

namespace Ayla
{
	class NotImplementedException : public Exception
	{
	public:
		NotImplementedException()
			: Exception(TEXT("The function to call does not implemented."))
		{
		}
	};
}