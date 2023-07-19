// Copyright 2020-2022 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

import Core;
import Core.Platform.Windows;

#endif

namespace
{
	static size_t NumWorkerThreads;
	static size_t NumCompletionPortThreads;
	static ScheduledIOContext sScheduledContext;

	static Spinlock Lock;
	static SpinlockConditionVariable ConditionVar;
	static std::queue<std::function<void()>> Works;

	void WorkerThreadRun(size_t Index)
	{
		PlatformMisc::UnreferencedParameter(Index);

		std::shared_ptr Context = SynchronizationContext::Install();

		while (true)
		{
			std::unique_lock ScopedLock(Lock);
			ConditionVar.Wait(ScopedLock, [&Context]()
			{
				return Works.empty() == false || Context->IsQueued();
			});

			std::function<void()> Work;
			if (Works.empty())
			{
				// Context is thread-local storage.
				Work = Context->Dequeue();
			}
			else
			{
				Work = std::move(Works.front());
				Works.pop();
			}

			ScopedLock.unlock();

			if (Work)
			{
				Work();
			}
		}

		SynchronizationContext::Uninstall(std::move(Context));
	}

#if PLATFORM_WINDOWS
	WindowsIOCPImplementation Implementation;

	void Worker_WindowsIOCP()
	{
		while (true)
		{
			Implementation.poll_one();
		}
	}
#endif

	void TrapInitialize()
	{
		static int TrapView = []()
		{
			NumWorkerThreads = std::thread::hardware_concurrency();
			NumCompletionPortThreads = std::thread::hardware_concurrency();

			for (size_t i = 0; i < NumCompletionPortThreads; ++i)
			{
				std::thread([Index = i]
				{
					PlatformMisc::SetThreadName(String::Format(TEXT("IOCP #{}"), Index));
					Worker_WindowsIOCP();
				}).detach();
			}

			for (size_t i = 0; i < NumWorkerThreads; ++i)
			{
				std::thread([Index = i]
				{
					PlatformMisc::SetThreadName(String::Format(TEXT("Worker #{}"), Index));
					WorkerThreadRun(Index);
				}).detach();
			}

			std::thread([]
			{
				PlatformMisc::SetThreadName(TEXT("Timer #0"));
				sScheduledContext.Run();
			}).detach();

			return 0;
		}();
	}
}

void ThreadPool::BindHandle(void* NativeHandle)
{
	static int Trap_view = (TrapInitialize(), 0);
#if PLATFORM_WINDOWS
	Implementation.bind_handle(NativeHandle);
#endif
}

void ThreadPool::QueueUserWorkItem(Work_t InWork)
{
	static int Trap_view = (TrapInitialize(), 0);

	std::unique_lock ScopedLock(Lock);
	Works.emplace(std::move(InWork));
	ConditionVar.NotifyOne();
}

void ThreadPool::QueueDelayedUserWorkItem(std::chrono::nanoseconds InDur, Work_t InWork)
{
	static int Trap_view = (TrapInitialize(), 0);
	sScheduledContext.Post(InDur, std::move(InWork));
}

void ThreadPool::NotifyCustom()
{
	ConditionVar.NotifyAll();
}

void ThreadPool::GetMinThreads(size_t& OutWorkerThreads, size_t& OutCompletionPortThreads)
{
	OutWorkerThreads = NumWorkerThreads;
	OutCompletionPortThreads = NumCompletionPortThreads;
}

void ThreadPool::GetMaxThreads(size_t& OutWorkerThreads, size_t& OutCompletionPortThreads)
{
	OutWorkerThreads = NumWorkerThreads;
	OutCompletionPortThreads = NumCompletionPortThreads;
}