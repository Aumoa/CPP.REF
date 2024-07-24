// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "Platform/PlatformMacros.h"

export module Core:GenericPlatformProcess;

export import :StaticClass;

export class CORE_API GenericPlatformProcess : public StaticClass
{
	static FORCEINLINE bool IsDebuggerPresent() noexcept
	{
		return false;
	}
};