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

	if (CompletedWork)
	{
		CompletedWork();
	}

	Works.clear();
	CompletedWork = nullptr;
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
	_Thread.bRunning = false;

	// Flush executing event.
	ExecuteWorks(nullptr);

	// Join executing thread.
	_Thread.Thread.join();
}

void RenderThread::EnqueueRenderThreadWork(size_t InWorkingHash, std::function<void()> InWorkBody)
{
	std::unique_lock lock(_Thread.CriticalSection);
	_WaitingWorks.Works.emplace_back(std::move(InWorkBody));
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