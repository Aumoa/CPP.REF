// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"
#include <functional>

namespace Ayla
{
	class CORE_API SynchronizationContext
	{
	public:
		template<class TBody>
#if __cpp_lib_move_only_function
		using function_t = std::move_only_function<TBody>;
#else
		using function_t = std::function<TBody>;
#endif

	public:
		virtual void Post(function_t<void()> continuation) = 0;

	public:
		static void SetSynchronizationContext(SynchronizationContext* context);
		static SynchronizationContext* GetCurrent();
	};
}