// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
//#include "CoreAssert.h"

template<class T>
class RefPtr
{
	T* _ptr = nullptr;
	ObjectReference* _reference = nullptr;

public:
	inline RefPtr() noexcept
	{
	}

	inline RefPtr(std::nullptr_t) noexcept
	{
	}

	inline RefPtr(T* ptr) noexcept
	{
		Reset(ptr);
	}

	inline RefPtr(const RefPtr& ptr) noexcept
	{
		Reset(ptr.Get());
	}

	inline RefPtr(RefPtr&& ptr) noexcept
	{
		Swap(ptr);
	}

	inline ~RefPtr() noexcept
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

	inline RefPtr& Reset(T* rhs = nullptr) noexcept
	{
		// Same pointer.
		if (_ptr == rhs)
		{
			return *this;
		}

		// Unreference pointer.
		if (_reference)
		{
			_reference->DecrRef();
			_reference = nullptr;
			_ptr = nullptr;
		}

		if (rhs != nullptr)
		{
			_reference = rhs->Reference;
			_reference->IncrRef();
			_ptr = rhs;
		}

		return *this;
	}

	inline RefPtr& Swap(RefPtr& rhs) noexcept
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

	inline RefPtr& operator =(const RefPtr& rhs) noexcept
	{
		return Reset(rhs.Get());
	}

	inline RefPtr& operator =(RefPtr&& rhs) noexcept
	{
		Reset();
		return Swap(rhs);
	}

	inline auto operator <=>(const RefPtr& rhs) const noexcept
	{
		return Get() <=> rhs.Get();
	}

	inline auto operator ==(const RefPtr& rhs) const noexcept
	{
		return Get() == rhs.Get();
	}
};

template<class T>
RefPtr(T*) -> RefPtr<T>;