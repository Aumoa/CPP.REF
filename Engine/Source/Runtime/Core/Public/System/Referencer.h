// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformAtomics.h"
#include "Platform/PlatformMacros.h"

class CORE_API Referencer
{
	volatile int32 Locks = 0;
	volatile int32 Weaks = 0;

public:
	FORCEINLINE constexpr Referencer() noexcept
	{
	}

	FORCEINLINE ~Referencer() noexcept
	{
	}

	FORCEINLINE void IncrRef() noexcept
	{
		IncrWeak();
		PlatformAtomics::InterlockedIncrement(&Locks);
	}

	FORCEINLINE bool DecrRef() noexcept
	{
		int32 Loc = PlatformAtomics::InterlockedDecrement(&Locks);
		if (DecrWeak())
		{
			delete this;
		}
		return Loc == 0;
	}

	FORCEINLINE void IncrWeak() noexcept
	{
		PlatformAtomics::InterlockedIncrement(&Weaks);
	}

	FORCEINLINE bool DecrWeak() noexcept
	{
		if (PlatformAtomics::InterlockedDecrement(&Weaks) == 0)
		{
			delete this;
			return true;
		}
		return false;
	}

	FORCEINLINE bool IsLived() const noexcept
	{
		return Locks > 0;
	}
};