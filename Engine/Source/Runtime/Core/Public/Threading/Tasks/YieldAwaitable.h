// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Threading/Tasks/YieldAwaiter.h"

namespace Ayla
{
	template<class T>
	class [[nodiscard]] Task;

	class YieldAwaitable
	{
	public:
		YieldAwaiter GetAwaiter() const noexcept
		{
			return YieldAwaiter();
		}
	};
}