// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
//#include "CoreAssert.h"

template<class T>
class WeakPtr
{
	T* _ptr = nullptr;
	ObjectReference* _reference = nullptr;

public:
	inline WeakPtr() noexcept
	{
	}

	inline WeakPtr(std::nullptr_t) noexcept
	{
	}

	inline WeakPtr(T* ptr) noexcept
	{
		Reset(ptr);
	}

	inline WeakPtr(const WeakPtr& ptr) noexcept
	{
		Reset(ptr.Get());
	}

	inline WeakPtr(WeakPtr&& ptr) noexcept
	{
		Swap(ptr);
	}

	inline ~WeakPtr() noexcept
	{
		Reset();
	}

	inline bool IsValid() const noexcept
	{
		if (_reference == nullptr)
		{
			return false;
		}

		return !_reference->bDisposed;
	}

	inline T* Get() const noexcept
	{
		if (IsValid())
		{
			return _ptr;
		}

		return nullptr;
	}

	inline WeakPtr& Reset(T* rhs = nullptr) noexcept
	{
		// Same pointer.
		if (_ptr == rhs)
		{
			return *this;
		}

		// Unreference pointer.
		if (_reference)
		{
			_reference->DecrWeak();
			_reference = nullptr;
			_ptr = nullptr;
		}

		if (rhs != nullptr)
		{
			_reference = rhs->Reference;
			_reference->IncrWeak();
			_ptr = rhs;
		}

		return *this;
	}

	inline WeakPtr& Swap(WeakPtr& rhs) noexcept
	{
		std::swap(_ptr, rhs._ptr);
		std::swap(_reference, rhs._reference);
		return *this;
	}

	inline operator bool() const noexcept
	{
		return IsValid();
	}

	inline T* operator ->() const noexcept
	{
		auto* p = Get();
		check(p);
		return p;
	}

	inline WeakPtr& operator =(const WeakPtr& rhs) noexcept
	{
		return Reset(rhs.Get());
	}

	inline WeakPtr& operator =(WeakPtr&& rhs) noexcept
	{
		Reset();
		return Swap(rhs);
	}
};

template<class T>
WeakPtr(T*) -> WeakPtr<T>;