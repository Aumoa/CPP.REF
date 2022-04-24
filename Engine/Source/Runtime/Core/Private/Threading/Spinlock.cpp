// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Threading/Spinlock.h"

using namespace ::libty;

#if PLATFORM_WINDOWS

#include <Windows.h>
#undef GetMessage

struct Spinlock::Impl
{
	SRWLOCK _lock;

	inline Impl() noexcept
	{
		InitializeSRWLock(&_lock);
	}

	inline void _Lock()
	{
		AcquireSRWLockExclusive(&_lock);
	}

	inline void _Unlock()
	{
		ReleaseSRWLockExclusive(&_lock);
	}

	inline bool _Try_lock()
	{
		return TryAcquireSRWLockExclusive(&_lock) == TRUE;
	}

	inline void _Readonly_lock()
	{
		AcquireSRWLockShared(&_lock);
	}

	inline void _Readonly_unlock()
	{
		ReleaseSRWLockShared(&_lock);
	}

	inline bool _Readonly_try_lock()
	{
		return TryAcquireSRWLockShared(&_lock) == TRUE;
	}
};

#endif

Spinlock::Spinlock()
	: _impl(new Impl())
{
}

Spinlock::~Spinlock() noexcept
{
	delete _impl;
}

void Spinlock::Lock() noexcept
{
	_impl->_Lock();
}

void Spinlock::Unlock() noexcept
{
	_impl->_Unlock();
}

bool Spinlock::TryLock() noexcept
{
	return _impl->_Try_lock();
}

void Spinlock::LockReadonly() noexcept
{
	_impl->_Readonly_lock();
}

void Spinlock::UnlockReadonly() noexcept
{
	_impl->_Readonly_unlock();
}

bool Spinlock::TryLockReadonly() noexcept
{
	return _impl->_Readonly_try_lock();
}

void* Spinlock::NativeHandle() const noexcept
{
	return &_impl->_lock;
}