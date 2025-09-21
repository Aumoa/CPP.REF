// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Threading/ThreadPool.h"
#include "Threading/Thread.h"
#include "Platform/PlatformIO.h"
#include "ScriptingBackend/ScriptingBackend.h"

namespace Ayla
{
	void (*ThreadPool::coreclr__QueueUserWorkItem)();
	void (*ThreadPool::coreclr__GetMinThreads)(int32* workerThreads, int32* completionPortThreads);
	void (*ThreadPool::coreclr__GetMaxThreads)(int32* workerThreads, int32* completionPortThreads);
	void (*ThreadPool::coreclr__SetMinThreads)(int32 workerThreads, int32 completionPortThreads);
	void (*ThreadPool::coreclr__SetMaxThreads)(int32 workerThreads, int32 completionPortThreads);

	size_t ThreadPool::NumCompletionPortThreads;

	Spinlock ThreadPool::Lck;
	SpinlockConditionVariable ThreadPool::Cv;
	std::queue<Action<>> ThreadPool::Works;

	Spinlock ThreadPool::DelayedLck;
	SpinlockConditionVariable ThreadPool::DelayedCv;
	std::multimap<std::chrono::steady_clock::time_point, Action<>> ThreadPool::DelayedWorks;

	void* ThreadPool::IO;
	size_t ThreadPool::IOCPWorkers;
	std::vector<std::thread> ThreadPool::Threads;

	template<class Ret, class... Args>
	inline auto fget(std::string_view name)
	{
		return reinterpret_cast<Ret(*)(Args...)>(ScriptingBackend::Get().GetFunctionPointer("Core.Script", "Ayla.ThreadPoolMarshal", name));
	}

	void ThreadPool::static__ThreadPool()
	{
		static class s_trap_init
		{
		public:
			s_trap_init()
			{
				coreclr__QueueUserWorkItem = fget<void>("QueueUserWorkItem");
				coreclr__GetMinThreads = fget<void, int32*, int32*>("GetMinThreads");
				coreclr__GetMaxThreads = fget<void, int32*, int32*>("GetMaxThreads");
				coreclr__SetMinThreads = fget<void, int32, int32>("SetMinThreads");
				coreclr__SetMaxThreads = fget<void, int32, int32>("SetMaxThreads");

				check(IO == nullptr);
				PlatformIO::InitializeIOCPHandle(IO);
				NumCompletionPortThreads = 4;

				while (IOCPWorkers < NumCompletionPortThreads)
				{
					Threads.emplace_back(std::bind(IOCPWorker, IOCPWorkers++));
				}

				Threads.emplace_back(DelayedWorker);
			}

			~s_trap_init() noexcept
			{
				auto handle = IO;
				auto lock1 = std::unique_lock{ Lck };
				auto lock2 = std::unique_lock{ DelayedLck };
				IO = nullptr;
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

	void ThreadPool::QueueUserWorkItem(Action<> InWork)
	{
		static__ThreadPool();
		std::unique_lock lock{ Lck };
		Works.emplace(std::move(InWork));
		lock.unlock();
		coreclr__QueueUserWorkItem();
	}

	void ThreadPool::QueueDelayedUserWorkItem(std::chrono::nanoseconds InDur, Action<> InWork)
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
		coreclr__GetMinThreads(workerThreads, completionPortThreads);
	}

	void ThreadPool::GetMaxThreads(int32* workerThreads, int32* completionPortThreads)
	{
		static__ThreadPool();
		coreclr__GetMaxThreads(workerThreads, completionPortThreads);
	}

	void ThreadPool::SetMinThreads(int32 workerThreads, int32 completionPortThreads)
	{
		static__ThreadPool();
		coreclr__SetMinThreads(workerThreads, completionPortThreads);
	}

	void ThreadPool::SetMaxThreads(int32 workerThreads, int32 completionPortThreads)
	{
		static__ThreadPool();
		coreclr__SetMaxThreads(workerThreads, completionPortThreads);
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
		Action<> work = std::move(Works.front());
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