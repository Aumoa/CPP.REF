// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "Object.h"

namespace SC::Runtime::Core
{
	class HashHelper abstract final : virtual public Core::Object
	{
	public:
		static size_t GetHashCode(double value);
	};
}