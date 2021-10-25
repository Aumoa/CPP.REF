// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <utility>
#include "Object.h"

template<class T>
class ScopedPtr
{
	template<class O>
	friend class ScopedPtr;
	T* _ptr = nullptr;

	ScopedPtr(const ScopedPtr&) = delete;

public:
	ScopedPtr()
	{
	}

	ScopedPtr(T* ptr) : _ptr(ptr)
	{
	}

	template<class O>
	ScopedPtr(ScopedPtr<O>&& rhs) : _ptr(rhs._ptr)
	{
		rhs._ptr = nullptr;
	}

	~ScopedPtr()
	{
		InternalDestroy();
	}

	T* Get() const
	{
		return _ptr;
	}

	T* operator ->() const
	{
		return _ptr;
	}

	bool IsValid() const
	{
		return _ptr != nullptr;
	}

	operator bool() const
	{
		return _ptr != nullptr;
	}

	T* Detach()
	{
		T* ptr = _ptr;
		_ptr = nullptr;
		return ptr;
	}

	void Attach(T* ptr)
	{
		InternalDestroy();
		_ptr = ptr;
	}

	void Swap(T*& rhs)
	{
		std::swap(_ptr, rhs);
	}

	void Swap(ScopedPtr& rhs)
	{
		std::swap(_ptr, rhs._ptr);
	}

	template<class O>
	ScopedPtr& operator =(ScopedPtr<O>&& rhs)
	{
		InternalDestroy();
		_ptr = rhs._ptr;
		rhs._ptr = nullptr;
		return *this;
	}

	bool operator ==(const T* rhs) const
	{
		return _ptr == rhs;
	}

	bool operator !=(const T* rhs) const
	{
		return _ptr != rhs;
	}

	template<class O>
	bool operator ==(const ScopedPtr<O>& rhs) const
	{
		return _ptr == rhs._ptr;
	}

	template<class O>
	bool operator !=(const ScopedPtr<O>& rhs) const
	{
		return _ptr != rhs._ptr;
	}

private:
	void InternalDestroy()
	{
		if (_ptr)
		{
			if (SObject* Outer = _ptr->GetOuter())
			{
				Outer->DestroyObject(_ptr);
			}
			_ptr = nullptr;
		}
	}
};