// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Threading/ThreadPool.h"
#include "IO/IOContext.h"
#include "IO/ScheduledIOContext.h"
#include "Misc/String.h"
#include <thread>

namespace
{
	static IOContext sContext;
	static ScheduledIOContext sScheduledContext;
}

void ThreadPool::BindHandle(void* nativeHandle)
{
	static int trap = (_trap_init(), 0);
	sContext.BindHandle(nativeHandle);
}

void ThreadPool::QueueUserWorkItem(Work_t work)
{
	static int trap = (_trap_init(), 0);
	sContext.Post(std::move(work));
}

void ThreadPool::QueueDelayedUserWorkItem(std::chrono::nanoseconds dur, Work_t work)
{
	static int trap = (_trap_init(), 0);
	sScheduledContext.Post(dur, std::move(work));
}

void ThreadPool::_trap_init()
{
	static int trap = []()
	{
		for (size_t i = 0; i < std::thread::hardware_concurrency(); ++i)
		{
			std::thread([index = i]
			{
				PlatformMisc::SetThreadName(TEXTF("IOCP #{}", index));
				sContext.Run();
			}).detach();
		}
		
		std::thread([]
		{
			PlatformMisc::SetThreadName(TEXT("Timer #0"));
			sScheduledContext.Run(sContext);
		}).detach();
		return 0;
	}();
}