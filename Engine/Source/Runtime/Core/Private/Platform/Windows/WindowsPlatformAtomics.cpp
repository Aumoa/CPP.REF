// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Platform/Windows/WindowsPlatformAtomics.h"

#if PLATFORM_WINDOWS

#define __ALLOW_PLATFORM_COMMON_H__

#include "Math_.h"
#include "Platform/PlatformCommon.h"

namespace Ayla
{
	inline SRWLOCK* GetSRWLOCK(void*& LockVal)
	{
		if constexpr (sizeof(void*) >= sizeof(SRWLOCK))
		{
			return &reinterpret_cast<SRWLOCK&>(LockVal);
		}
		else
		{
			return (SRWLOCK*)LockVal;
		}
	}

	inline CONDITION_VARIABLE* GetCONDITION_VARIABLE(void*& CondVal)
	{
		if constexpr (sizeof(void*) >= sizeof(CONDITION_VARIABLE))
		{
			return &reinterpret_cast<CONDITION_VARIABLE&>(CondVal);
		}
		else
		{
			return (CONDITION_VARIABLE*)CondVal;
		}
	}

	void WindowsPlatformAtomics::InitializeSpinlock(void*& LockVal) noexcept
	{
		if constexpr (sizeof(void*) >= sizeof(SRWLOCK))
		{
			::InitializeSRWLock(&(SRWLOCK&)LockVal);
		}
		else
		{
			auto* Ptr = new SRWLOCK();
			::InitializeSRWLock(Ptr);
			LockVal = Ptr;
		}
	}

	void WindowsPlatformAtomics::DestroySpinlock(void*& LockVal) noexcept
	{
		if constexpr (sizeof(void*) >= sizeof(SRWLOCK))
		{
			__nop();
		}
		else
		{
			delete (SRWLOCK*)LockVal;
		}
	}

	void WindowsPlatformAtomics::AcquireSpinlock(void*& LockVal, bool bShared) noexcept
	{
		if (bShared)
		{
			::AcquireSRWLockShared(GetSRWLOCK(LockVal));
		}
		else
		{
			::AcquireSRWLockExclusive(GetSRWLOCK(LockVal));
		}
	}

	void WindowsPlatformAtomics::ReleaseSpinlock(void*& LockVal, bool bShared) noexcept
	{
		if (bShared)
		{
			::ReleaseSRWLockShared(GetSRWLOCK(LockVal));
		}
		else
		{
			::ReleaseSRWLockExclusive(GetSRWLOCK(LockVal));
		}
	}

	bool WindowsPlatformAtomics::TryAcquireSpinlock(void*& LockVal, bool bShared) noexcept
	{
		if (bShared)
		{
			return ::TryAcquireSRWLockShared(GetSRWLOCK(LockVal)) == TRUE;
		}
		else
		{
			return ::TryAcquireSRWLockExclusive(GetSRWLOCK(LockVal)) == TRUE;
		}
	}

	void WindowsPlatformAtomics::InitializeSpinlockConditionVariable(void*& CondVal) noexcept
	{
		if constexpr (sizeof(void*) >= sizeof(CONDITION_VARIABLE))
		{
			::InitializeConditionVariable(&(CONDITION_VARIABLE&)CondVal);
		}
		else
		{
			auto* Ptr = new CONDITION_VARIABLE();
			::InitializeConditionVariable(Ptr);
			CondVal = Ptr;
		}
	}

	void WindowsPlatformAtomics::DestroySpinlockConditionVariable(void*& CondVal) noexcept
	{
		if constexpr (sizeof(void*) >= sizeof(CONDITION_VARIABLE))
		{
			__nop();
		}
		else
		{
			delete (CONDITION_VARIABLE*)CondVal;
		}
	}

	void WindowsPlatformAtomics::WaitSpinlockConditionVariable(void*& CondVal, void*& LockVal, bool bShared) noexcept
	{
		ULONG Flags = bShared ? CONDITION_VARIABLE_LOCKMODE_SHARED : 0;
		::SleepConditionVariableSRW(GetCONDITION_VARIABLE(CondVal), GetSRWLOCK(LockVal), INFINITE, Flags);
	}

	bool WindowsPlatformAtomics::WaitForSpinlockConditionVariable(void*& CondVal, void*& LockVal, size_t Sleep, bool bShared) noexcept
	{
		ULONG Flags = bShared ? CONDITION_VARIABLE_LOCKMODE_SHARED : 0;
		Sleep = Math::Min(Sleep, (size_t)std::numeric_limits<ULONG>::max());
		return ::SleepConditionVariableSRW(GetCONDITION_VARIABLE(CondVal), GetSRWLOCK(LockVal), (ULONG)Sleep, Flags) == TRUE;
	}

	void WindowsPlatformAtomics::NotifyOneSpinlockConditionVariable(void*& CondVal) noexcept
	{
		::WakeConditionVariable(GetCONDITION_VARIABLE(CondVal));
	}

	void WindowsPlatformAtomics::NotifyAllSpinlockConditionVariable(void*& CondVal) noexcept
	{
		::WakeAllConditionVariable(GetCONDITION_VARIABLE(CondVal));
	}
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif