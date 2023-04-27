// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core.Threading:Spinlock;

export import Core.Platform;

export class Spinlock
{
	struct Impl;
	Spinlock(const Spinlock&) = delete;

public:
	class Readonly_t
	{
		Spinlock* Src;

	public:
		Readonly_t(Spinlock* SSrc)
			: Src(SSrc)
		{
		}

		inline void lock() noexcept(noexcept(Src->Lock(true)))
		{
			Src->Lock(true);
		}

		inline void unlock() noexcept(noexcept(Src->Unlock(true)))
		{
			Src->Unlock(true);
		}

		inline size_t& NativeHandle() noexcept(noexcept(Src->NativeHandle()))
		{
			return Src->NativeHandle();
		}
	};

private:
	size_t LockVal = 0;
	Readonly_t Readonly;

public:
	inline Spinlock() noexcept
		: Readonly(this)
	{
		PlatformMisc::InitializeSpinlock(LockVal);
	}

	inline Spinlock(Spinlock&& r) noexcept
		: Readonly(this)
	{
		std::swap(LockVal, r.LockVal);
	}

	inline ~Spinlock() noexcept
	{
		if (LockVal != 0)
		{
			PlatformMisc::DestroySpinlock(LockVal);
			LockVal = 0;
		}
	}

	inline void Lock(bool shared = false) noexcept
	{
		if (shared)
		{
			PlatformMisc::AcquireSpinlockShared(LockVal);
		}
		else
		{
			PlatformMisc::AcquireSpinlockExclusive(LockVal);
		}
	}

	inline void Unlock(bool shared = false) noexcept
	{
		if (shared)
		{
			PlatformMisc::ReleaseSpinlockShared(LockVal);
		}
		else
		{
			PlatformMisc::ReleaseSpinlockExclusive(LockVal);
		}
	}

	inline bool TryLock(bool shared = false) noexcept
	{
		if (shared)
		{
			return PlatformMisc::TryAcquireSpinlockShared(LockVal);
		}
		else
		{
			return PlatformMisc::TryAcquireSpinlockExclusive(LockVal);
		}
	}

	inline Spinlock& operator =(Spinlock&& r) noexcept
	{
		std::swap(LockVal, r.LockVal);
		return *this;
	}

	inline size_t& NativeHandle() noexcept
	{
		return LockVal;
	}

	inline Readonly_t& AsReadonly() noexcept
	{
		return Readonly;
	}

	inline void lock() noexcept { Lock(); }
	inline void unlock() noexcept { Unlock(); }
};