// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "CoreAssert.h"

template<class T>
class WeakPtr
{
	T* _ptr;
	ObjectReference* _reference;

public:
	inline WeakPtr(T* ptr) noexcept
	{
		_ptr = ptr;
		if (ptr)
		{
			_reference = ptr->Reference;
			_reference->IncrWeak();
		}
	}

	inline WeakPtr(const WeakPtr& ptr) noexcept
	{
		_ptr = ptr._ptr;
		if (_ptr)
		{
			_reference = _ptr->Reference;
			_reference->IncrWeak();
		}
	}

	inline WeakPtr(WeakPtr&& ptr) noexcept
	{
		_ptr = ptr._ptr;
		_reference = ptr._reference;
		ptr._ptr = nullptr;
		ptr._reference = nullptr;
	}

	inline ~WeakPtr() noexcept
	{
		if (_reference)
		{
			_reference->DecrWeak();
			_reference = nullptr;
		}
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
};

template<class T>
WeakPtr(T*) -> WeakPtr<T>;