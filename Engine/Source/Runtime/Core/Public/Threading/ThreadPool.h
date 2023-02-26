// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreTypes/IntegralTypes.h"
#include <functional>
#include <chrono>

class CORE_API ThreadPool
{
public:
	using Work_t = std::function<void()>;

public:
	static void BindHandle(void* nativeHandle);
	static void QueueUserWorkItem(Work_t work);
	static void QueueDelayedUserWorkItem(std::chrono::nanoseconds dur, Work_t work);

private:
	static void _trap_init();
};