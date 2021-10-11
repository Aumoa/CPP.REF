// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GameThreads/RenderThread.h"
#include "Threading/EventHandle.h"

void RenderThread::PendingThreadWork::Init()
{
	ExecuteEvent = std::make_shared<SEventHandle>();
	CompletedEvent = std::make_shared<SEventHandle>();
	CompletedEvent->Set();
}

void RenderThread::PendingThreadWork::SwapExecute(WaitingThreadWorks& target)
{
	std::swap(Works, target.Works);
	std::swap(CompletedWork, target.CompletedWork);
	ExecuteEvent->Set();
}

void RenderThread::PendingThreadWork::RunningWorks_RenderThread()
{
	for (auto& pair : Works)
	{
		pair.second();
	}

	CompletedWork();
}

void RenderThread::ThreadInfo::Init()
{
	Thread = std::thread(std::bind(&ThreadInfo::Worker, &_thread));
	bRunning = true;
}

void RenderThread::ThreadInfo::Init_RenderThread()
{
	SThread* CurrentThread = SThread::GetCurrentThread();
	ThreadId = CurrentThread->GetThreadId();
	CurrentThread->SetFriendlyName(L"[Render Thread]");
}

void RenderThread::ThreadInfo::WaitToComplete()
{
	std::unique_lock lock(CriticalSection);
	bRunning = false;

	// Flush executing event.
	_executingWorks.ExecuteEvent->Set();

	// Join executing thread.
	lock.unlock();
	Thread.join();
}

void RenderThread::ThreadInfo::Worker()
{
	Init_RenderThread();

	while (bRunning)
	{
		_executingWorks.ExecuteEvent->Wait();
		{
			std::unique_lock lock(CriticalSection);
			_executingWorks.RunningWorks_RenderThread();
		}
		_executingWorks.CompletedEvent->Set();
	}
}

void RenderThread::Init()
{
	_thread.Init();
	_executingWorks.Init();
}

void RenderThread::Shutdown()
{
	_thread.WaitToComplete();
}

void RenderThread::EnqueueRenderThreadWork(size_t workingHash, std::function<void()> work)
{
	std::unique_lock lock(_thread.CriticalSection);
	_waitingWorks.Works.emplace(workingHash, work);
}

void RenderThread::ExecuteWorks(std::function<void()> completedWork)
{
	std::unique_lock lock(_thread.CriticalSection);
	_waitingWorks.CompletedWork = completedWork;
	_executingWorks.SwapExecute(_waitingWorks);
}

void RenderThread::WaitForLastWorks()
{
	_executingWorks.CompletedEvent->Wait();
}