// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GameThreads/RenderThread.h"
#include "Threading/EventHandle.h"

void RenderThread::PendingThreadWork::Init()
{
	ExecuteEvent = std::make_shared<SEventHandle>();
	CompletedEvent = std::make_shared<SEventHandle>();
	CompletedEvent->Set();
}

void RenderThread::PendingThreadWork::SwapExecute(WaitingThreadWorks& InTarget)
{
	std::swap(Works, InTarget.Works);
	std::swap(CompletedWork, InTarget.CompletedWork);
	ExecuteEvent->Set();
}

void RenderThread::PendingThreadWork::RunningWorks_RenderThread()
{
	for (auto& WorkBody : Works)
	{
		WorkBody();
	}

	CompletedWork();
}

void RenderThread::ThreadInfo::Init()
{
	Thread = std::thread(std::bind(&ThreadInfo::Worker, &_Thread));
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
	_ExecutingWorks.ExecuteEvent->Set();

	// Join executing thread.
	lock.unlock();
	Thread.join();
}

void RenderThread::ThreadInfo::Worker()
{
	Init_RenderThread();

	while (bRunning)
	{
		_ExecutingWorks.ExecuteEvent->Wait();
		{
			std::unique_lock lock(CriticalSection);
			_ExecutingWorks.RunningWorks_RenderThread();
		}
		_ExecutingWorks.CompletedEvent->Set();
	}
}

void RenderThread::Init()
{
	_Thread.Init();
	_ExecutingWorks.Init();
}

void RenderThread::Shutdown()
{
	_Thread.WaitToComplete();
}

void RenderThread::EnqueueRenderThreadWork(size_t InWorkingHash, std::function<void()> InWorkBody)
{
	std::unique_lock lock(_Thread.CriticalSection);
	_WaitingWorks.Works.emplace_back(InWorkBody);
}

void RenderThread::ExecuteWorks(std::function<void()> InCompletionWork)
{
	std::unique_lock lock(_Thread.CriticalSection);
	_WaitingWorks.CompletedWork = InCompletionWork;
	_ExecutingWorks.SwapExecute(_WaitingWorks);
}

void RenderThread::WaitForLastWorks()
{
	_ExecutingWorks.CompletedEvent->Wait();
}