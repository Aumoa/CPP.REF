// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Marshal/NativeExceptionInterop.h"
#include <utility>

namespace Ayla
{
	class CORE_API NativeCallBoundary
	{
	public:
		template<class Callable>
		static NativeCallStatus Invoke(Callable&& callable) noexcept
		{
			try
			{
				return std::forward<Callable>(callable)();
			}
			catch (...)
			{
				return NativeExceptionInterop::CaptureCurrentException();
			}
		}
	};
}
