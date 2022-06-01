// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include <mutex>
#include <utility>

class CORE_API Spinlock
{
	struct Impl;
	Spinlock(const Spinlock&) = delete;

private:
	Impl* _impl = nullptr;

public:
	Spinlock();
	~Spinlock() noexcept;

	void Lock() noexcept;
	void Unlock() noexcept;
	bool TryLock() noexcept;

	void LockReadonly() noexcept;
	void UnlockReadonly() noexcept;
	bool TryLockReadonly() noexcept;

	void* NativeHandle() const noexcept;

public:
	class Readonly_t
	{
	public:
		Readonly_t() = default;
	};

	inline static Readonly_t Readonly;
};

template<>
class std::unique_lock<Spinlock>
{
	unique_lock(const unique_lock&) = delete;

	Spinlock* _lck;
	uint8 _readonly : 1;
	uint8 _owns : 1;

public:
	unique_lock() noexcept
		: _lck(nullptr)
		, _readonly(false)
		, _owns(false)
	{
	}

	[[nodiscard]] explicit unique_lock(Spinlock& lck) noexcept
		: _lck(&lck)
		, _readonly(false)
		, _owns(true)
	{
		_lck->Lock();
	}

	[[nodiscard]] explicit unique_lock(Spinlock& lck, std::adopt_lock_t) noexcept
		: _lck(&lck)
		, _readonly(false)
		, _owns(true)
	{
	}

	[[nodiscard]] explicit unique_lock(Spinlock& lck, std::try_to_lock_t) noexcept
		: _lck(&lck)
		, _readonly(false)
		, _owns(lck.TryLock())
	{
	}

	[[nodiscard]] explicit unique_lock(Spinlock& lck, std::defer_lock_t) noexcept
		: _lck(&lck)
		, _readonly(false)
		, _owns(false)
	{
	}

	[[nodiscard]] explicit unique_lock(Spinlock& lck, Spinlock::Readonly_t isReadonly) noexcept
		: _lck(&lck)
		, _readonly(true)
		, _owns(true)
	{
		_lck->LockReadonly();
	}

	[[nodiscard]] explicit unique_lock(Spinlock& lck, Spinlock::Readonly_t isReadonly, std::adopt_lock_t) noexcept
		: _lck(&lck)
		, _readonly(true)
		, _owns(true)
	{
	}

	[[nodiscard]] explicit unique_lock(Spinlock& lck, Spinlock::Readonly_t isReadonly, std::try_to_lock_t) noexcept
		: _lck(&lck)
		, _readonly(true)
		, _owns(lck.TryLockReadonly())
	{
	}

	[[nodiscard]] explicit unique_lock(Spinlock& lck, Spinlock::Readonly_t isReadonly, std::defer_lock_t) noexcept
		: _lck(&lck)
		, _readonly(true)
		, _owns(false)
	{
	}

	[[nodiscard]] explicit unique_lock(unique_lock&& rhs) noexcept
		: _lck(rhs._lck)
		, _readonly(rhs._readonly)
		, _owns(rhs._owns)
	{
		rhs._lck = nullptr;
		rhs._readonly = false;
		rhs._owns = false;
	}

	~unique_lock() noexcept
	{
		if (_owns)
		{
			this->unlock();
		}
	}

	void lock()
	{
		if (_readonly)
		{
			_lck->LockReadonly();
		}
		else
		{
			_lck->Lock();
		}

		_owns = true;
	}

	bool try_lock()
	{
		if (_readonly)
		{
			_owns = _lck->TryLockReadonly();
		}
		else
		{
			_owns = _lck->TryLock();
		}
		return _owns;
	}

	void unlock()
	{
		if (_readonly)
		{
			_lck->UnlockReadonly();
		}
		else
		{
			_lck->Unlock();
		}

		_owns = false;
	}

	unique_lock& operator =(unique_lock&& rhs) noexcept
	{
		_lck = rhs._lck;
		_readonly = rhs._readonly;
		_owns = rhs._owns;
		rhs._lck = nullptr;
		rhs._readonly = false;
		rhs._owns = false;
		return *this;
	}

	Spinlock* release() noexcept
	{
		Spinlock* lck = _lck;
		_lck = nullptr;
		_readonly = false;
		_owns = false;
		return _lck;
	}

	[[nodiscard]] bool owns_lock() const noexcept
	{
		return _owns;
	}

	[[nodiscard]] bool readonly() const noexcept
	{
		return _readonly;
	}

	explicit operator bool() const noexcept
	{
		return _owns;
	}

	[[nodiscard]] Spinlock* mutex() const noexcept
	{
		return _lck;
	}
};

namespace std
{
	template<class T>
	std::unique_lock(T&, Spinlock::Readonly_t) -> std::unique_lock<Spinlock>;

	template<class T>
	std::unique_lock(T&, Spinlock::Readonly_t, std::adopt_lock_t) -> std::unique_lock<Spinlock>;

	template<class T>
	std::unique_lock(T&, Spinlock::Readonly_t, std::try_to_lock_t) -> std::unique_lock<Spinlock>;

	template<class T>
	std::unique_lock(T&, Spinlock::Readonly_t, std::defer_lock_t) -> std::unique_lock<Spinlock>;
}