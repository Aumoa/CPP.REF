// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "Misc/PlatformMisc.h"

class Spinlock
{
	struct Impl;
	Spinlock(const Spinlock&) = delete;

public:
	class Readonly_t
	{
		Spinlock* _src;

	public:
		Readonly_t(Spinlock* src)
			: _src(src)
		{
		}

		inline void lock() noexcept(noexcept(_src->Lock(true)))
		{
			_src->Lock(true);
		}

		inline void unlock() noexcept(noexcept(_src->Unlock(true)))
		{
			_src->Unlock(true);
		}

		inline size_t& NativeHandle() noexcept(noexcept(_src->NativeHandle()))
		{
			return _src->NativeHandle();
		}
	};

private:
	size_t _lock = 0;
	Readonly_t _readonly;

public:
	inline Spinlock() noexcept
		: _readonly(this)
	{
		PlatformMisc::InitializeSpinlock(_lock);
	}

	inline Spinlock(Spinlock&& r) noexcept
		: _readonly(this)
	{
		std::swap(_lock, r._lock);
	}

	inline ~Spinlock() noexcept
	{
		if (_lock != 0)
		{
			PlatformMisc::DestroySpinlock(_lock);
			_lock = 0;
		}
	}

	inline void Lock(bool shared = false) noexcept
	{
		if (shared)
		{
			PlatformMisc::AcquireSpinlockShared(_lock);
		}
		else
		{
			PlatformMisc::AcquireSpinlockExclusive(_lock);
		}
	}

	inline void Unlock(bool shared = false) noexcept
	{
		if (shared)
		{
			PlatformMisc::ReleaseSpinlockShared(_lock);
		}
		else
		{
			PlatformMisc::ReleaseSpinlockExclusive(_lock);
		}
	}

	inline bool TryLock(bool shared = false) noexcept
	{
		if (shared)
		{
			return PlatformMisc::TryAcquireSpinlockShared(_lock);
		}
		else
		{
			return PlatformMisc::TryAcquireSpinlockExclusive(_lock);
		}
	}

	inline Spinlock& operator =(Spinlock&& r) noexcept
	{
		std::swap(_lock, r._lock);
		return *this;
	}

	inline size_t& NativeHandle() noexcept
	{
		return _lock;
	}

	inline Readonly_t& AsReadonly() noexcept
	{
		return _readonly;
	}

	inline void lock() noexcept { Lock(); }
	inline void unlock() noexcept { Unlock(); }
};