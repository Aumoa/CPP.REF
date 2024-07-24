// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "System/AssertionMacros.h"

export module Core:Spinlock;

export import :PlatformAtomics;

export class Spinlock
{
	Spinlock(const Spinlock&) = delete;
	Spinlock(Spinlock&&) = delete;

	Spinlock& operator =(const Spinlock&) = delete;
	Spinlock& operator =(Spinlock&&) = delete;

private:
	void* LockVal = 0;

public:
	FORCEINLINE Spinlock() noexcept
	{
		PlatformAtomics::InitializeSpinlock(LockVal);
	}

	FORCEINLINE ~Spinlock() noexcept
	{
		PlatformAtomics::DestroySpinlock(LockVal);
		LockVal = nullptr;
	}

	FORCEINLINE void Lock(bool bShared = false) noexcept
	{
		PlatformAtomics::AcquireSpinlock(LockVal, bShared);
	}

	FORCEINLINE void Unlock(bool bShared = false) noexcept
	{
		PlatformAtomics::ReleaseSpinlock(LockVal, bShared);
	}

	FORCEINLINE bool TryLock(bool bShared = false) noexcept
	{
		return PlatformAtomics::TryAcquireSpinlock(LockVal, bShared);
	}

	FORCEINLINE void*& NativeHandle() noexcept
	{
		return LockVal;
	}

	FORCEINLINE const void* NativeHandle() const noexcept
	{
		return LockVal;
	}

	FORCEINLINE void lock() noexcept
	{
		Lock();
	}

	FORCEINLINE void unlock() noexcept
	{
		Unlock();
	}

	FORCEINLINE bool try_lock() noexcept
	{
		return TryLock();
	}

	FORCEINLINE void lock_shared() noexcept
	{
		Lock(true);
	}

	FORCEINLINE bool try_lock_shared() noexcept
	{
		return TryLock(true);
	}

	FORCEINLINE void unlock_shared() noexcept
	{
		Unlock(true);
	}
};