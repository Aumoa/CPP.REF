// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformAtomics.h"
#include "System/TimeSpan.h"
#include <chrono>
#include <shared_mutex>

class CORE_API SpinlockConditionVariable
{
	using MyClock = std::chrono::steady_clock;

	SpinlockConditionVariable(const SpinlockConditionVariable&) = delete;
	SpinlockConditionVariable(SpinlockConditionVariable&&) = delete;

	SpinlockConditionVariable& operator =(const SpinlockConditionVariable&) = delete;
	SpinlockConditionVariable& operator =(SpinlockConditionVariable&&) = delete;

private:
	void* CondVal = 0;

public:
	inline SpinlockConditionVariable() noexcept
	{
		PlatformAtomics::InitializeSpinlockConditionVariable(CondVal);
	}

	inline ~SpinlockConditionVariable() noexcept
	{
		PlatformAtomics::DestroySpinlockConditionVariable(CondVal);
	}

	inline void NotifyOne() noexcept
	{
		PlatformAtomics::NotifyOneSpinlockConditionVariable(CondVal);
	}

	inline void NotifyAll() noexcept
	{
		PlatformAtomics::NotifyAllSpinlockConditionVariable(CondVal);
	}

	template<class TSpinlock>
	inline void Wait(std::unique_lock<TSpinlock>& Lck) noexcept
	{
		PlatformAtomics::WaitSpinlockConditionVariable(CondVal, Lck.mutex()->NativeHandle(), false);
	}

	template<class TSpinlock>
	inline void Wait(std::shared_lock<TSpinlock>& Lck) noexcept
	{
		PlatformAtomics::WaitSpinlockConditionVariable(CondVal, Lck.mutex()->NativeHandle(), true);
	}

	template<class TSpinlock>
	inline bool WaitFor(std::unique_lock<TSpinlock>& Lck, const TimeSpan& Dur) noexcept
	{
		return PlatformAtomics::WaitForSpinlockConditionVariable(CondVal, Lck.mutex()->NativeHandle(), (size_t)Dur.GetTotalMilliseconds(), false);
	}

	template<class TSpinlock>
	inline bool WaitFor(std::shared_lock<TSpinlock>& Lck, const TimeSpan& Dur) noexcept
	{
		return PlatformAtomics::WaitForSpinlockConditionVariable(CondVal, Lck.mutex()->NativeHandle(), (size_t)Dur.GetTotalMilliseconds(), true);
	}

	template<class TSpinlock>
	inline bool WaitUntil(std::unique_lock<TSpinlock>& Lck, MyClock::time_point Tp) noexcept
	{
		return WaitFor(Lck, std::chrono::duration_cast<std::chrono::milliseconds>(Tp - MyClock::now()));
	}

	template<class TSpinlock>
	inline bool WaitUntil(std::shared_lock<TSpinlock>& Lck, MyClock::time_point Tp) noexcept
	{
		return WaitFor(Lck, std::chrono::duration_cast<std::chrono::milliseconds>(Tp - MyClock::now()));
	}

public:
	template<class TSpinlock, class TPredicate>
	inline void Wait(std::unique_lock<TSpinlock>& Lck, TPredicate Pred) noexcept
	{
		while (!Pred())
		{
			Wait(Lck);
		}
	}

	template<class TSpinlock, class TPredicate>
	inline void Wait(std::shared_lock<TSpinlock>& Lck, TPredicate Pred) noexcept
	{
		while (!Pred())
		{
			Wait(Lck);
		}
	}

	template<class TSpinlock, class TPredicate>
	inline bool WaitFor(std::unique_lock<TSpinlock>& Lck, const TimeSpan& Dur, TPredicate Pred) noexcept
	{
		return WaitUntil(Lck, MyClock::now() + (std::chrono::milliseconds)Dur, std::move(Pred));
	}

	template<class TSpinlock, class TPredicate>
	inline bool WaitFor(std::shared_lock<TSpinlock>& Lck, const TimeSpan& Dur, TPredicate Pred) noexcept
	{
		return WaitUntil(Lck, MyClock::now() + (std::chrono::milliseconds)Dur, std::move(Pred));
	}

	template<class TSpinlock, class TPredicate>
	inline bool WaitUntil(std::unique_lock<TSpinlock>& Lck, MyClock::time_point Tp, TPredicate Pred) noexcept
	{
		while (!Pred())
		{
			auto now = MyClock::now();
			if (Tp <= now)
			{
				return false;
			}

			if (this->WaitUntil(Lck, Tp))
			{
				return true;
			}
		}

		return true;
	}

	template<class TSpinlock, class TPredicate>
	inline bool WaitUntil(std::shared_lock<TSpinlock>& Lck, MyClock::time_point Tp, TPredicate Pred) noexcept
	{
		while (!Pred())
		{
			auto now = MyClock::now();
			if (Tp <= now)
			{
				return false;
			}

			if (this->WaitUntil(Lck, Tp))
			{
				return true;
			}
		}

		return true;
	}
};