// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include <atomic>

struct ObjectReference
{
	int32 Shared;
	int32 Weak;
	bool bDisposed;

	inline int32 IncrRef() noexcept
	{
		int32 ref = ++Shared;
		IncrWeak();
		return ref;
	}

	inline int32 DecrRef() noexcept
	{
		int32 ref = --Shared;
		DecrWeak();
		return ref;
	}

	inline int32 IncrWeak() noexcept
	{
		return ++Weak;
	}

	inline int32 DecrWeak() noexcept
	{
		int32 ref = --Weak;
		if (ref == 0)
		{
			delete this;
		}
		return ref;
	}
};