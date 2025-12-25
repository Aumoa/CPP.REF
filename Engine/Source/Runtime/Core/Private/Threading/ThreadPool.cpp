// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Threading/ThreadPool.h"
#include "Threading/Thread.h"
#include "Platform/PlatformIO.h"
#include <thread>

namespace Ayla
{
	int32 ThreadPool::MinWorkerThreads = 1;
	int32 ThreadPool::MaxWorkerThreads = 32767;
	int32 ThreadPool::MinCompletionPortThreads = 1;
	int32 ThreadPool::MaxCompletionPortThreads = 1000;
	size_t ThreadPool::NumWorkerThreads = 0;
	size_t ThreadPool::NumCompletionPortThreads = 0;

	Spinlock ThreadPool::Lck;
	SpinlockConditionVariable ThreadPool::Cv;
	std::queue<ThreadPool::function_t<void()>> ThreadPool::Works;

	Spinlock ThreadPool::DelayedLck;
	SpinlockConditionVariable ThreadPool::DelayedCv;
	std::multimap<std::chrono::steady_clock::time_point, ThreadPool::function_t<void()>> ThreadPool::DelayedWorks;

	void* ThreadPool::IO = nullptr;
	std::vector<std::thread> ThreadPool::Threads;
	bool ThreadPool::bShutdown = false;

	void ThreadPool::static__ThreadPool()
	{
		static class s_trap_init
		{
		public:
			s_trap_init()
			{
				check(IO == nullptr);
				PlatformIO::InitializeIOCPHandle(IO);
				
				// Initialize default thread counts
				size_t hardwareConcurrency = std::thread::hardware_concurrency();
				if (hardwareConcurrency == 0)
				{
					hardwareConcurrency = 4;
				}
				
				NumWorkerThreads = hardwareConcurrency;
				NumCompletionPortThreads = 4;
				
				// Start worker threads
				for (size_t i = 0; i < NumWorkerThreads; ++i)
				{
					Threads.emplace_back(std::bind(WorkerThread, i));
				}

				// Start IOCP worker threads
				for (size_t i = 0; i < NumCompletionPortThreads; ++i)
				{
					Threads.emplace_back(std::bind(IOCPWorker, i));
				}

				// Start delayed worker thread
				Threads.emplace_back(DelayedWorker);
			}

			~s_trap_init() noexcept
			{
				auto handle = IO;
				auto lock1 = std::unique_lock{ Lck };
				auto lock2 = std::unique_lock{ DelayedLck };
				IO = nullptr;
				bShutdown = true;
				Cv.NotifyAll();
				DelayedCv.NotifyAll();
				lock1.unlock();
				lock2.unlock();

				PlatformIO::QueueInterruptSignal(handle, (int32)NumCompletionPortThreads);
				for (auto& thread : Threads)
				{
					if (thread.joinable())
					{
						thread.join();
					}
				}

				PlatformIO::DestroyIOCPHandle(handle);
			}
		} _;
	}

	void ThreadPool::BindHandle(void* NativeHandle)
	{
		static__ThreadPool();
		PlatformIO::BindIOHandle(IO, NativeHandle);
	}

	void ThreadPool::UnbindHandle(void* NativeHandle)
	{
		static__ThreadPool();
		PlatformIO::UnbindIOHandle(IO, NativeHandle);
	}

	void ThreadPool::QueueUserWorkItem(function_t<void()> InWork)
	{
		static__ThreadPool();
		std::unique_lock lock{ Lck };
		Works.emplace(std::move(InWork));
		lock.unlock();
		Cv.NotifyOne();
	}

	void ThreadPool::QueueDelayedUserWorkItem(std::chrono::nanoseconds InDur, function_t<void()> InWork)
	{
		static__ThreadPool();
		auto Tp = std::chrono::steady_clock::now() + InDur;
		std::unique_lock ScopedLock(DelayedLck);
		DelayedWorks.emplace(Tp, std::move(InWork));
		DelayedCv.NotifyOne();
	}

	void ThreadPool::QueueSignal()
	{
		static__ThreadPool();
		std::unique_lock lock1(Lck);
		std::unique_lock lock2(DelayedLck);
		Cv.NotifyAll();
		DelayedCv.NotifyAll();
	}

	void ThreadPool::GetMinThreads(int32* workerThreads, int32* completionPortThreads)
	{
		static__ThreadPool();
		if (workerThreads)
		{
			*workerThreads = MinWorkerThreads;
		}
		if (completionPortThreads)
		{
			*completionPortThreads = MinCompletionPortThreads;
		}
	}

	void ThreadPool::GetMaxThreads(int32* workerThreads, int32* completionPortThreads)
	{
		static__ThreadPool();
		if (workerThreads)
		{
			*workerThreads = MaxWorkerThreads;
		}
		if (completionPortThreads)
		{
			*completionPortThreads = MaxCompletionPortThreads;
		}
	}

	void ThreadPool::SetMinThreads(int32 workerThreads, int32 completionPortThreads)
	{
		static__ThreadPool();
		MinWorkerThreads = workerThreads;
		MinCompletionPortThreads = completionPortThreads;
	}

	void ThreadPool::SetMaxThreads(int32 workerThreads, int32 completionPortThreads)
	{
		static__ThreadPool();
		MaxWorkerThreads = workerThreads;
		MaxCompletionPortThreads = completionPortThreads;
	}

	void ThreadPool::WorkerThread(size_t Index)
	{
		String name = String::Format(TEXT("Worker #{}"), Index);
		Thread::GetCurrentThread().SetDescription(name);

		while (!bShutdown)
		{
			std::unique_lock lock{ Lck };
			Cv.Wait(lock, []()
			{
				return !Works.empty() || bShutdown;
			});

			if (bShutdown)
			{
				break;
			}

			if (!Works.empty())
			{
				function_t<void()> work = std::move(Works.front());
				Works.pop();
				lock.unlock();
				
				work();
			}
		}

		PlatformProcess::OutputDebugString(String::Format(TEXT("{0} closed."), name));
	}

	void ThreadPool::IOCPWorker(size_t Index)
	{
		String name = String::Format(TEXT("IOCP #{}"), Index);
		Thread::GetCurrentThread().SetDescription(name);

		while (IO)
		{
			PlatformIO::DispatchQueuedCompletionStatus(IO);
		}

		PlatformProcess::OutputDebugString(String::Format(TEXT("{0} closed."), name));
	}

	void ThreadPool::DelayedWorker()
	{
		std::vector<function_t<void()>> Actions;
		
		String name = TEXT("Timer #0");
		Thread::GetCurrentThread().SetDescription(name);

		while (!bShutdown)
		{
			// calculate timer works.
			{
				std::unique_lock ScopedLock(DelayedLck);
				if (bShutdown)
				{
					break;
				}
				DelayedCv.Wait(ScopedLock, []()
				{
					return !DelayedWorks.empty() || bShutdown;
				});
				if (bShutdown)
				{
					break;
				}

				auto It = DelayedWorks.begin();
				std::chrono::steady_clock::time_point Until = It->first;
				if (It->first > std::chrono::steady_clock::now())
				{
					DelayedCv.WaitUntil(ScopedLock, Until);
				}
				if (bShutdown)
				{
					break;
				}

				Actions.clear();
				while (DelayedWorks.size() > 0)
				{
					It = DelayedWorks.begin();
					if (It->first > std::chrono::steady_clock::now())
					{
						// Not yet.
						break;
					}

					Actions.emplace_back(std::move(It->second));
					DelayedWorks.erase(It);
				}

				ScopedLock.unlock();
			}

			// enqueue to user work item.
			{
				std::unique_lock ScopedLock(Lck);
				for (auto& Action : Actions)
				{
					Works.emplace(std::move(Action));
					Cv.NotifyOne();
				}
			}
		}
		
		PlatformProcess::OutputDebugString(String::Format(TEXT("{0} closed."), name));
	}
}