// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"

namespace Ayla
{
	class CORE_API ArgumentOutOfRangeException : public Exception
	{
	public:
		ArgumentOutOfRangeException();
	};
}