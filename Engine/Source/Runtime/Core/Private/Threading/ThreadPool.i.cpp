// Copyright 2020-2022 Aumoa.lib. All right reserved.

import Core;

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

void ThreadPool::Initialize(size_t InNumWorkerThreads, size_t InNumCompletionPortThreads)
{
	static int Init = ([&]()
	{
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

		UpdateWorkers();
		UpdateIOCPWorkers();
		std::thread(DelayedWorker).detach();
	}(), 0);
}

void ThreadPool::BindHandle(void* NativeHandle)
{
	Initialize(0, 0);
	PlatformIO::BindIOHandle(IO, NativeHandle);
}

void ThreadPool::UnbindHandle(void* NativeHandle)
{
	Initialize(0, 0);
	PlatformIO::UnbindIOHandle(IO, NativeHandle);
}

void ThreadPool::QueueUserWorkItem(Action<> InWork)
{
	Initialize(0, 0);
	std::unique_lock ScopedLock(Lck);
	Works.emplace(std::move(InWork));
	Cv.NotifyOne();
}

void ThreadPool::QueueDelayedUserWorkItem(std::chrono::nanoseconds InDur, Action<> InWork)
{
	Initialize(0, 0);
	auto Tp = std::chrono::steady_clock::now() + InDur;
	std::unique_lock ScopedLock(DelayedLck);
	DelayedWorks.emplace(Tp, std::move(InWork));
	DelayedCv.NotifyOne();
}

void ThreadPool::GetMinThreads(size_t& OutWorkerThreads, size_t& OutCompletionPortThreads)
{
	Initialize(0, 0);
	OutWorkerThreads = NumWorkerThreads;
	OutCompletionPortThreads = NumCompletionPortThreads;
}

void ThreadPool::GetMaxThreads(size_t& OutWorkerThreads, size_t& OutCompletionPortThreads)
{
	Initialize(0, 0);
	OutWorkerThreads = NumWorkerThreads;
	OutCompletionPortThreads = NumCompletionPortThreads;
}

void ThreadPool::UpdateWorkers()
{
	std::unique_lock ScopedLock(Lck);

	while (Workers < NumWorkerThreads)
	{
		std::thread(std::bind(Worker, Workers++)).detach();
	}
}

void ThreadPool::UpdateIOCPWorkers()
{
	std::unique_lock ScopedLock(Lck);

	while (IOCPWorkers < NumCompletionPortThreads)
	{
		std::thread(std::bind(IOCPWorker, IOCPWorkers++)).detach();
	}
}

void ThreadPool::Worker(size_t Index)
{
	PLATFORM_UNREFERENCED_PARAMETER(Index);

	Thread::GetCurrentThread().SetDescription(String::Format(TEXT("Worker #{}"), Index));

	while (true)
	{
		std::unique_lock ScopedLock(Lck);
		Cv.Wait(ScopedLock, []() { return !Works.empty(); });

		Action<> MyAction = std::move(Works.front());
		Works.pop();

		ScopedLock.unlock();
		MyAction();
	}
}

void ThreadPool::IOCPWorker(size_t Index)
{
	PLATFORM_UNREFERENCED_PARAMETER(Index);

	Thread::GetCurrentThread().SetDescription(String::Format(TEXT("IOCP #{}"), Index));

	while (true)
	{
		PlatformIO::DispatchQueuedCompletionStatus(IO);
	}
}

void ThreadPool::DelayedWorker()
{
	std::vector<Action<>> Actions;

	while (true)
	{
		// calculate timer works.
		{
			std::unique_lock ScopedLock(DelayedLck);
			DelayedCv.Wait(ScopedLock, []() { return !DelayedWorks.empty(); });

			auto It = DelayedWorks.begin();
			std::chrono::steady_clock::time_point Until = It->first;
			if (It->first > std::chrono::steady_clock::now())
			{
				DelayedCv.WaitUntil(ScopedLock, Until);
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
}