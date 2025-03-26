// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformAtomics.h"
#include "Platform/PlatformMacros.h"

namespace Ayla
{
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
			DecrWeak();
			return Loc == 0;
		}

		FORCEINLINE bool TryIncrRef() noexcept
		{
			int32 Uses = PlatformAtomics::AtomicRead(&Locks);
			while (Uses != 0)
			{
				const int32 OldValue = PlatformAtomics::InterlockedCompareExchange(&Locks, Uses + 1, Uses);
				if (OldValue == Uses)
				{
					return true;
				}

				Uses = OldValue;
			}
			return false;
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

		FORCEINLINE bool IsLived() const noexcept { return Locks > 0; }
		FORCEINLINE int32 GetLocks() const noexcept { return Locks; }
		FORCEINLINE int32 GetWeaks() const noexcept { return Weaks; }
	};
}