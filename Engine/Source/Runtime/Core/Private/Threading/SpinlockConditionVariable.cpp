// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Threading/SpinlockConditionVariable.h"

using namespace ::libty;

#if PLATFORM_WINDOWS

#include <Windows.h>
#undef GetMessage

struct SpinlockConditionVariable::Impl
{
	CONDITION_VARIABLE _var;

	inline Impl() noexcept
	{
		InitializeConditionVariable(&_var);
	}

	inline void NotifyAll() noexcept
	{
		WakeAllConditionVariable(&_var);
	}

	inline void NotifyOne() noexcept
	{
		WakeConditionVariable(&_var);
	}

	inline bool Wait(void* lck, uint32 millis, bool readonly) noexcept
	{
		return SleepConditionVariableSRW(&_var, (PSRWLOCK)lck, millis, readonly ? CONDITION_VARIABLE_LOCKMODE_SHARED : 0) == TRUE;
	}
};

#endif

SpinlockConditionVariable::SpinlockConditionVariable()
	: _impl(new Impl())
{
}

SpinlockConditionVariable::~SpinlockConditionVariable()
{
	delete _impl;
}

void SpinlockConditionVariable::NotifyOne() noexcept
{
	_impl->NotifyOne();
}

void SpinlockConditionVariable::NotifyAll() noexcept
{
	_impl->NotifyAll();
}

void SpinlockConditionVariable::Wait(std::unique_lock<Spinlock>& lck) noexcept
{
	_impl->Wait(lck.mutex()->NativeHandle(), 0xFFFFFFFF, lck.readonly());
}

bool SpinlockConditionVariable::WaitFor(std::unique_lock<Spinlock>& lck, std::chrono::milliseconds dur) noexcept
{
	return _impl->Wait(lck.mutex()->NativeHandle(), (uint32)dur.count(), lck.readonly());
}

bool SpinlockConditionVariable::WaitUntil(std::unique_lock<Spinlock>& lck, _Clock::time_point tp) noexcept
{
	return _impl->Wait(lck.mutex()->NativeHandle(), (uint32)std::chrono::duration_cast<std::chrono::milliseconds>(_Clock::now() - tp).count(), lck.readonly());
}