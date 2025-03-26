// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "StaticClass.h"
#include "Platform/PlatformMacros.h"
#include <shared_mutex>
#include <condition_variable>
#include <chrono>

namespace Ayla
{
	struct CORE_API GenericPlatformAtomics : public StaticClass
	{
		static FORCEINLINE void InitializeSpinlock(void*& LockVal) noexcept
		{
			LockVal = new std::shared_mutex();
		}

		static FORCEINLINE void DestroySpinlock(void*& LockVal) noexcept
		{
			delete (std::shared_mutex*)LockVal;
		}

		static FORCEINLINE void AcquireSpinlock(void*& LockVal, bool bShared) noexcept
		{
			if (bShared)
			{
				((std::shared_mutex*)LockVal)->lock_shared();
			}
			else
			{
				((std::shared_mutex*)LockVal)->lock();
			}
		}

		static FORCEINLINE void ReleaseSpinlock(void*& LockVal, bool bShared) noexcept
		{
			if (bShared)
			{
				((std::shared_mutex*)LockVal)->unlock_shared();
			}
			else
			{
				((std::shared_mutex*)LockVal)->unlock();
			}
		}

		static FORCEINLINE bool TryAcquireSpinlock(void*& LockVal, bool bShared) noexcept
		{
			if (bShared)
			{
				return ((std::shared_mutex*)LockVal)->try_lock_shared();
			}
			else
			{
				return ((std::shared_mutex*)LockVal)->try_lock();
			}
		}

		static FORCEINLINE void InitializeSpinlockConditionVariable(void*& CondVal)
		{
			CondVal = new std::condition_variable_any();
		}

		static FORCEINLINE void DestroySpinlockConditionVariable(void*& CondVal) noexcept
		{
			delete (std::condition_variable_any*)CondVal;
		}

		static FORCEINLINE void WaitSpinlockConditionVariable(void*& CondVal, void*& LockVal, bool) noexcept
		{
			((std::condition_variable_any*)CondVal)->wait(*(std::shared_mutex*)LockVal);
		}

		static FORCEINLINE bool WaitForSpinlockConditionVariable(void*& CondVal, void*& LockVal, size_t Sleep, bool) noexcept
		{
			return ((std::condition_variable_any*)CondVal)->wait_for(*(std::shared_mutex*)LockVal, std::chrono::milliseconds(Sleep)) == std::cv_status::no_timeout;
		}

		static FORCEINLINE void NotifyOneSpinlockConditionVariable(void*& CondVal) noexcept
		{
			((std::condition_variable_any*)CondVal)->notify_one();
		}

		static FORCEINLINE void NotifyAllSpinlockConditionVariable(void*& CondVal) noexcept
		{
			((std::condition_variable_any*)CondVal)->notify_all();
		}
	};
}