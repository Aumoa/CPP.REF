// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include <atomic>

struct Referencer
{
	std::atomic<bool> bDisposed = false;
	std::atomic<int32> References = 0;
	std::atomic<int32> WeakReferences = 0;

	inline Referencer()
	{
	}

	inline bool IsValid() const volatile
	{
		return !bDisposed;
	}
};