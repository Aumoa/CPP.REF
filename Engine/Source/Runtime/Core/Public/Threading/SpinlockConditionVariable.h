// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreTypes/IntegralTypes.h"
#include "Spinlock.h"
#include "CoreTypes/TimeSpan.h"
#include "Misc/PlatformMisc.h"
#include <chrono>
#include <mutex>

class CORE_API SpinlockConditionVariable
{
	using _Clock = std::chrono::steady_clock;
	SpinlockConditionVariable(const SpinlockConditionVariable&) = delete;

private:
	size_t _cv = 0;

public:
	inline SpinlockConditionVariable() noexcept
	{
		PlatformMisc::InitializeConditionVariable(_cv);
	}

	inline ~SpinlockConditionVariable() noexcept
	{
		if (_cv != 0)
		{
			PlatformMisc::DestroySpinlock(_cv);
			_cv = 0;
		}
	}

	inline void NotifyOne() noexcept
	{
		PlatformMisc::NotifyOneConditionVariable(_cv);
	}

	inline void NotifyAll() noexcept
	{
		PlatformMisc::NotifyAllConditionVariable(_cv);
	}

	template<class TSpinlock>
	inline void Wait(TSpinlock& lck, bool shared = false) noexcept
	{
		PlatformMisc::SleepConditionVariable(_cv, lck.NativeHandle(), 0xFFFFFFFF, shared);
	}

	template<class TSpinlock>
	inline void Wait(std::unique_lock<TSpinlock>& lck) noexcept
	{
		PlatformMisc::SleepConditionVariable(_cv, lck.mutex()->NativeHandle(), 0xFFFFFFFF, false);
	}

	template<class TSpinlock>
	inline bool WaitFor(TSpinlock& lck, const TimeSpan& dur, bool shared = false) noexcept
	{
		return PlatformMisc::SleepConditionVariable(_cv, lck.NativeHandle(), (uint32)dur.GetTotalMilliseconds(), shared);
	}

	template<class TSpinlock>
	inline bool WaitFor(std::unique_lock<TSpinlock>& lck, const TimeSpan& dur) noexcept
	{
		return PlatformMisc::SleepConditionVariable(_cv, lck.mutex()->NativeHandle(), (uint32)dur.GetTotalMilliseconds(), false);
	}

	template<class TSpinlock>
	inline bool WaitUntil(TSpinlock& lck, _Clock::time_point tp, bool shared = false) noexcept
	{
		return WaitFor(lck, std::chrono::duration_cast<std::chrono::milliseconds>(tp - _Clock::now()), shared);
	}

	template<class TSpinlock>
	inline bool WaitUntil(std::unique_lock<TSpinlock>& lck, _Clock::time_point tp) noexcept
	{
		return WaitFor(lck, std::chrono::duration_cast<std::chrono::milliseconds>(tp - _Clock::now()));
	}

public:
	template<class TSpinlock, class TPredicate>
	inline void Wait(std::unique_lock<TSpinlock>& lck, TPredicate pred) noexcept
	{
		while (!pred())
		{
			Wait(lck);
		}
	}

	template<class TSpinlock, class TPredicate>
	inline bool WaitFor(std::unique_lock<TSpinlock>& lck, const TimeSpan& dur, TPredicate pred) noexcept
	{
		return WaitUntil(lck, _Clock::now() + (std::chrono::milliseconds)dur, std::move(pred));
	}

	template<class TSpinlock, class TPredicate>
	inline bool WaitUntil(std::unique_lock<TSpinlock>& lck, _Clock::time_point tp, TPredicate pred) noexcept
	{
		while (!pred())
		{
			auto now = _Clock::now();
			if (tp <= now)
			{
				return false;
			}

			if (this->WaitUntil(lck, tp))
			{
				return true;
			}
		}

		return true;
	}
};