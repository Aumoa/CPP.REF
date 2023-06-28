// Copyright 2020-2022 Aumoa.lib. All right reserved.

import Core;

#if PLATFORM_WINDOWS

import Core.Platform.Windows;

#endif

namespace
{
	static IOContext sContext;
	static ScheduledIOContext sScheduledContext;

	void TrapInitialize()
	{
		static int TrapView = []()
		{
			for (size_t i = 0; i < std::thread::hardware_concurrency(); ++i)
			{
				std::thread([index = i]
				{
					PlatformMisc::SetThreadName(String::Format(TEXT("IOCP #{}"), index));
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
}

void ThreadPool::BindHandle(void* NativeHandle)
{
	static int Trap_view = (TrapInitialize(), 0);
	sContext.BindHandle(NativeHandle);
}

void ThreadPool::QueueUserWorkItem(Work_t InWork)
{
	static int Trap_view = (TrapInitialize(), 0);
	sContext.Post([InWork](size_t, int32) { InWork(); });
}

void ThreadPool::QueueDelayedUserWorkItem(std::chrono::nanoseconds InDur, Work_t InWork)
{
	static int Trap_view = (TrapInitialize(), 0);
	sScheduledContext.Post(InDur, [InWork](size_t, int32) { InWork(); });
}