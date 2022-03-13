// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "IMulticastDelegate.h"
#include <memory>

class ScopedDelegateHolder
{
	friend class DelegateHandle;
	template<class>
	friend class MulticastDelegate;

	IMulticastDelegate* _delegate = nullptr;
	std::weak_ptr<int64> _idptr;
	int64 _id;

public:
	ScopedDelegateHolder() = default;
	ScopedDelegateHolder(const ScopedDelegateHolder&) = default;
	ScopedDelegateHolder(ScopedDelegateHolder&&) = default;

	ScopedDelegateHolder(IMulticastDelegate* delegate, std::shared_ptr<int64> id)
		: _delegate(delegate)
		, _idptr(id)
		, _id(*id)
	{
	}

	bool IsValid() const
	{
		if (_idptr.expired())
		{
			return false;
		}

		return _delegate && _id != 0;
	}

	void Reset()
	{
		_delegate = nullptr;
		_idptr.reset();
		_id = 0;
	}

	IMulticastDelegate* operator ->() const
	{
		return _delegate;
	}

	auto operator <=>(const ScopedDelegateHolder& rhs) const { return _id <=> rhs._id; }
	bool operator ==(const ScopedDelegateHolder& rhs) const { return _id == rhs._id; }

	ScopedDelegateHolder& operator =(const ScopedDelegateHolder&) = default;
	ScopedDelegateHolder& operator =(ScopedDelegateHolder&&) = default;
};