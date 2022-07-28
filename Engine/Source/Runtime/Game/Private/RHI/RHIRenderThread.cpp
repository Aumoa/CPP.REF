// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/RHIRenderThread.h"
#include "Threading/GameThreads.h"
#include "RHIRenderThread.gen.cpp"

RHIRenderThread::RHIRenderThread()
{
	_running = true;
	_thread = Thread::CreateThread(TEXT("[RHI]"), std::bind(&RHIRenderThread::Worker, this));
}

void RHIRenderThread::StopThread(bool join)
{
	std::unique_lock lock(_mutex);
	if (_running == false)
	{
		// Already stopped.
		return;
	}

	_running = false;
	_cv.NotifyOne();

	if (join)
	{
		lock.unlock();
		_thread.JoinAsync().Wait();
	}
}

void RHIRenderThread::EnqueueWork(std::function<void()> work)
{
	check(GameThreads::IsInGameThread());
	check(work);
	_pendingWorks.emplace_back(std::move(work));
}

Task<> RHIRenderThread::ExecuteWorks(std::function<void()> work)
{
	TaskCompletionSource tcs = TaskCompletionSource<>::Create();

	// Locking for make waiting operation to atomic.
	std::unique_lock lock(_mutex);

	{
		// Ready pending works.
		std::unique_lock lock(_lockWorks);
		_readyWorks.clear();
		_pendingWorks.swap(_readyWorks);

		// Add work to execute last work and signal.
		_readyWorks.emplace_back([this, tcs, work]() mutable
		{
			if (work)
			{
				work();
			}

#if !SHIPPING
			_ensureWorks = false;
#endif
			tcs.SetResult();
		});
	}

#if !SHIPPING
	checkf(_ensureWorks == false, TEXT("ExecuteWorks() must be called after previous task is completed."));
	_ensureWorks = true;
#endif

	// Starting with render thread worker.
	_cv.NotifyOne();

	return tcs.GetTask();
}

void RHIRenderThread::Worker()
{
	std::vector<std::function<void()>> executeWorks;

	while (_running)
	{
		{
			std::unique_lock lock(_mutex);
			_cv.Wait(lock, [this]() { return !_running || !_readyWorks.empty(); });
		}

		{
			// Copy ready works to local storage.
			std::unique_lock lock(_lockWorks);
			executeWorks.swap(_readyWorks);
		}

		for (auto& work : executeWorks)
		{
			work();
		}

		executeWorks.clear();
	}
}