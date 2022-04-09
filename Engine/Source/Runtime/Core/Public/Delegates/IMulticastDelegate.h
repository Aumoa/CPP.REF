// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"

namespace libty::inline Core
{
	class DelegateHandle;

	class IMulticastDelegate
	{
	public:
		virtual ~IMulticastDelegate() noexcept = default;
		virtual void Remove(DelegateHandle&) = 0;
	};
}