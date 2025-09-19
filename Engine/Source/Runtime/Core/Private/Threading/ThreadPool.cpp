// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Threading/ThreadPool.h"
#include "Threading/Thread.h"
#include "Platform/PlatformIO.h"
#include "ScriptingBackend/ScriptingBackend.h"

namespace Ayla
{
	void (*ThreadPool::coreclr__QueueUserWorkItem)();
	void (*ThreadPool::coreclr__GetMaxThreads)(int32* workerThreads, int32* completionPortThreads);

	size_t ThreadPool::NumWorkerThreads;
	size_t ThreadPool::NumCompletionPortThreads;

	Spinlock ThreadPool::Lck;
	SpinlockConditionVariable ThreadPool::Cv;
	std::queue<Action<>> ThreadPool::Works;

	Spinlock ThreadPool::DelayedLck;
	SpinlockConditionVariable ThreadPool::DelayedCv;
	std::multimap<std::chrono::steady_clock::time_point, Action<>> ThreadPool::DelayedWorks;

	void* ThreadPool::IO;
	size_t ThreadPool::Workers;
	size_t ThreadPool::IOCPWorkers;
	std::vector<std::thread> ThreadPool::Threads;

	void ThreadPool::Initialize(size_t InNumWorkerThreads, size_t InNumCompletionPortThreads)
	{
		coreclr__QueueUserWorkItem = reinterpret_cast<void(*)()>(ScriptingBackend::Get().GetFunctionPointer("Core.Script", "Ayla.ThreadPool", "QueueUserWorkItem"));
		coreclr__GetMaxThreads = reinterpret_cast<void(*)(int32*, int32*)>(ScriptingBackend::Get().GetFunctionPointer("Core.Script", "Ayla.ThreadPool", "GetMaxThreads"));

		static int Init = ([&]()
		{
			check(IO == nullptr);
			PlatformIO::InitializeIOCPHandle(IO);

			if (InNumWorkerThreads == 0)
			{
				InNumWorkerThreads = std::thread::hardware_concurrency();
			}
			if (InNumCompletionPortThreads == 0)
			{
				InNumCompletionPortThreads = 4;
			}

			NumWorkerThreads = InNumWorkerThreads;
			NumCompletionPortThreads = InNumCompletionPortThreads;
			
			while (IOCPWorkers < NumCompletionPortThreads)
			{
				Threads.emplace_back(std::bind(IOCPWorker, IOCPWorkers++));
			}

			Threads.emplace_back(DelayedWorker);
		}(), 0);
	}

	void ThreadPool::Shutdown()
	{
		auto handle = IO;
		auto lock1 = std::unique_lock{ Lck };
		auto lock2 = std::unique_lock{ DelayedLck };
		IO = nullptr;
		Cv.NotifyAll();
		DelayedCv.NotifyAll();
		lock1.unlock();
		lock2.unlock();

		PlatformIO::QueueInterruptSignal(handle);
		for (auto& thread : Threads)
		{
			if (thread.joinable())
			{
				thread.join();
			}
		}

		PlatformIO::DestroyIOCPHandle(handle);
	}

	void ThreadPool::BindHandle(void* NativeHandle)
	{
		check(IO);
		PlatformIO::BindIOHandle(IO, NativeHandle);
	}

	void ThreadPool::UnbindHandle(void* NativeHandle)
	{
		check(IO);
		PlatformIO::UnbindIOHandle(IO, NativeHandle);
	}

	void ThreadPool::QueueUserWorkItem(Action<> InWork)
	{
		check(IO);
		std::unique_lock lock{ Lck };
		Works.emplace(std::move(InWork));
		lock.unlock();
		coreclr__QueueUserWorkItem();
		//Cv.NotifyOne();
	}

	void ThreadPool::QueueSignal()
	{
		check(IO);
		std::unique_lock lock1(Lck);
		std::unique_lock lock2(DelayedLck);
		Cv.NotifyAll();
		DelayedCv.NotifyAll();
	}

	void ThreadPool::QueueDelayedUserWorkItem(std::chrono::nanoseconds InDur, Action<> InWork)
	{
		check(IO);
		auto Tp = std::chrono::steady_clock::now() + InDur;
		std::unique_lock ScopedLock(DelayedLck);
		DelayedWorks.emplace(Tp, std::move(InWork));
		DelayedCv.NotifyOne();
	}

	void ThreadPool::GetMinThreads(size_t& OutWorkerThreads, size_t& OutCompletionPortThreads)
	{
		OutWorkerThreads = NumWorkerThreads;
		OutCompletionPortThreads = NumCompletionPortThreads;
	}

	void ThreadPool::GetMaxThreads(size_t& OutWorkerThreads, size_t& OutCompletionPortThreads)
	{
		int32 workerThreads, completionPortThreads;
		coreclr__GetMaxThreads(&workerThreads, &completionPortThreads);
		OutWorkerThreads = (size_t)workerThreads;
		OutCompletionPortThreads = NumCompletionPortThreads;
	}

	void ThreadPool::Worker(size_t Index)
	{
		PLATFORM_UNREFERENCED_PARAMETER(Index);

		String name = String::Format(TEXT("Worker #{}"), Index);
		Thread::GetCurrentThread().SetDescription(name);

		while (IO)
		{
			std::unique_lock ScopedLock(Lck);
			if (IO == nullptr)
			{
				break;
			}
			Cv.Wait(ScopedLock, []()
			{
				return !Works.empty() || IO == nullptr;
			});
			if (IO == nullptr)
			{
				break;
			}

			Action<> MyAction = std::move(Works.front());
			Works.pop();

			ScopedLock.unlock();
			MyAction();
		}

		PlatformProcess::OutputDebugString(String::Format(TEXT("{0} closed."), name));
	}

	void ThreadPool::IOCPWorker(size_t Index)
	{
		PLATFORM_UNREFERENCED_PARAMETER(Index);

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
		std::vector<Action<>> Actions;
		
		String name = TEXT("Timer #0");
		Thread::GetCurrentThread().SetDescription(name);

		while (IO)
		{
			// calculate timer works.
			{
				std::unique_lock ScopedLock(DelayedLck);
				if (IO == nullptr)
				{
					break;
				}
				DelayedCv.Wait(ScopedLock, []()
				{
					return !DelayedWorks.empty() || IO == nullptr;
				});
				if (IO == nullptr)
				{
					break;
				}

				auto It = DelayedWorks.begin();
				std::chrono::steady_clock::time_point Until = It->first;
				if (It->first > std::chrono::steady_clock::now())
				{
					DelayedCv.WaitUntil(ScopedLock, Until);
				}
				if (IO == nullptr)
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
				}
				Cv.NotifyOne();
			}
		}
		
		PlatformProcess::OutputDebugString(String::Format(TEXT("{0} closed."), name));
	}

	void ThreadPool::HandleUserWorkItem()
	{
		auto lock = std::unique_lock{ Lck };
		auto work = std::move(Works.front());
		Works.pop();
		lock.unlock();
		work();
	}
}

extern "C"
{
	PLATFORM_SHARED_EXPORT void Ayla__ThreadPool__HandleUserWorkItem()
	{
		::Ayla::ThreadPool::HandleUserWorkItem();
	}
}