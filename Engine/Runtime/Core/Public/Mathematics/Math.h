// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "Object.h"

namespace SC::Runtime::Core::Mathematics
{
	class CORE_API Math abstract final : virtual public Object
	{
	public:
		using Super = Object;
		using This = Math;
	};
}