// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "Spinlock.h"
#include <chrono>

class CORE_API SpinlockConditionVariable
{
	struct Impl;
	using _Clock = std::chrono::steady_clock;
	SpinlockConditionVariable(const SpinlockConditionVariable&) = delete;

private:
	Impl* _impl;

public:
	SpinlockConditionVariable();
	~SpinlockConditionVariable() noexcept;

	void NotifyOne() noexcept;
	void NotifyAll() noexcept;

	void Wait(std::unique_lock<Spinlock>& lck) noexcept;
	bool WaitFor(std::unique_lock<Spinlock>& lck, std::chrono::milliseconds dur) noexcept;
	bool WaitUntil(std::unique_lock<Spinlock>& lck, _Clock::time_point tp) noexcept;

public:
	template<class TPredicate>
	void Wait(std::unique_lock<Spinlock>& lck, TPredicate pred) noexcept
	{
		while (!pred())
		{
			Wait(lck);
		}
	}

	template<class TPredicate>
	bool WaitFor(std::unique_lock<Spinlock>& lck, std::chrono::milliseconds dur, TPredicate pred) noexcept
	{
		return WaitUntil(lck, _Clock::now() + dur, std::move(pred));
	}

	template<class TPredicate>
	bool WaitUntil(std::unique_lock<Spinlock>& lck, _Clock::time_point tp, TPredicate pred) noexcept
	{
		while (true)
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
	}
};