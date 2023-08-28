// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "System/StaticClass.h"
#include "Platform/PlatformMacros.h"

struct CORE_API GenericPlatformProcess : public StaticClass
{
	static FORCEINLINE void Yield()
	{
		PLATFORM_YIELD();
	}

	static FORCEINLINE void DebugBreak()
	{
		PLATFORM_BREAK();
	}

	static FORCEINLINE bool IsDebuggerPresent() noexcept
	{
		return false;
	}
};