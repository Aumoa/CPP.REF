// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core.Threading:SpinlockConditionVariable;

export import Core.Std;
export import Core.System;
export import Core.Platform;

export class CORE_API SpinlockConditionVariable
{
	using MyClock = std::chrono::steady_clock;
	SpinlockConditionVariable(const SpinlockConditionVariable&) = delete;

private:
	size_t Cval = 0;

public:
	inline SpinlockConditionVariable() noexcept
	{
		PlatformMisc::InitializeConditionVariable(Cval);
	}

	inline ~SpinlockConditionVariable() noexcept
	{
		if (Cval != 0)
		{
			PlatformMisc::DestroySpinlock(Cval);
			Cval = 0;
		}
	}

	inline void NotifyOne() noexcept
	{
		PlatformMisc::NotifyOneConditionVariable(Cval);
	}

	inline void NotifyAll() noexcept
	{
		PlatformMisc::NotifyAllConditionVariable(Cval);
	}

	template<class TSpinlock>
	inline void Wait(TSpinlock& Lck, bool bShared = false) noexcept
	{
		PlatformMisc::SleepConditionVariable(Cval, Lck.NativeHandle(), 0xFFFFFFFF, bShared);
	}

	template<class TSpinlock>
	inline void Wait(std::unique_lock<TSpinlock>& Lck) noexcept
	{
		PlatformMisc::SleepConditionVariable(Cval, Lck.mutex()->NativeHandle(), 0xFFFFFFFF, false);
	}

	template<class TSpinlock>
	inline bool WaitFor(TSpinlock& Lck, const TimeSpan& Dur, bool bShared = false) noexcept
	{
		return PlatformMisc::SleepConditionVariable(Cval, Lck.NativeHandle(), (uint32)Dur.GetTotalMilliseconds(), bShared);
	}

	template<class TSpinlock>
	inline bool WaitFor(std::unique_lock<TSpinlock>& Lck, const TimeSpan& Dur) noexcept
	{
		return PlatformMisc::SleepConditionVariable(Cval, Lck.mutex()->NativeHandle(), (uint32)Dur.GetTotalMilliseconds(), false);
	}

	template<class TSpinlock>
	inline bool WaitUntil(TSpinlock& Lck, MyClock::time_point tp, bool bShared = false) noexcept
	{
		return WaitFor(Lck, std::chrono::duration_cast<std::chrono::milliseconds>(tp - MyClock::now()), bShared);
	}

	template<class TSpinlock>
	inline bool WaitUntil(std::unique_lock<TSpinlock>& Lck, MyClock::time_point tp) noexcept
	{
		return WaitFor(Lck, std::chrono::duration_cast<std::chrono::milliseconds>(tp - MyClock::now()));
	}

public:
	template<class TSpinlock, class TPredicate>
	inline void Wait(std::unique_lock<TSpinlock>& Lck, TPredicate pred) noexcept
	{
		while (!pred())
		{
			Wait(Lck);
		}
	}

	template<class TSpinlock, class TPredicate>
	inline bool WaitFor(std::unique_lock<TSpinlock>& Lck, const TimeSpan& Dur, TPredicate pred) noexcept
	{
		return WaitUntil(Lck, MyClock::now() + (std::chrono::milliseconds)Dur, std::move(pred));
	}

	template<class TSpinlock, class TPredicate>
	inline bool WaitUntil(std::unique_lock<TSpinlock>& Lck, MyClock::time_point tp, TPredicate pred) noexcept
	{
		while (!pred())
		{
			auto now = MyClock::now();
			if (tp <= now)
			{
				return false;
			}

			if (this->WaitUntil(Lck, tp))
			{
				return true;
			}
		}

		return true;
	}
};