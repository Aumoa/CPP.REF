// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "RenderThread.h"

void RenderThread::PendingThreadWork::Init()
{
}

void RenderThread::PendingThreadWork::SwapExecute(IRHIDeviceContext* InDeviceContext, WaitingThreadWorks& InTarget)
{
	std::swap(Works, InTarget.Works);
	std::swap(CompletedWork, InTarget.CompletedWork);
	DeviceContext = InDeviceContext;
}

void RenderThread::PendingThreadWork::RunningWorks_RenderThread()
{
	for (auto& WorkBody : Works)
	{
		WorkBody(DeviceContext);
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
	bRunning = true;
	ThreadJoin = Thread::NewThread<void>(L"[Render Thread]", std::bind(&ThreadInfo::Worker, &_Thread));
}

void RenderThread::ThreadInfo::Init_RenderThread()
{
	::Thread* CurrentThread = ::Thread::GetCurrentThread();
	ThreadId = CurrentThread->GetThreadId();
	CurrentThread->SetFriendlyName(L"[Render Thread]");
}

void RenderThread::ThreadInfo::Worker()
{
	Init_RenderThread();

	std::unique_lock WorkerMtx_lock(WorkerMtx);

	while (bRunning)
	{
		cvWorker.wait(WorkerMtx_lock);
		_ExecutingWorks.RunningWorks_RenderThread();
		WorkerPromise.set_value();
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
	ExecuteWorks(nullptr, nullptr);

	// Join executing thread.
	_Thread.ThreadJoin.wait();
}

void RenderThread::EnqueueRenderThreadWork(size_t InWorkingHash, std::function<void(IRHIDeviceContext*)> InWorkBody)
{
	std::unique_lock lock(_Thread.WorkerMtx);
	_WaitingWorks.Works.emplace_back(std::move(InWorkBody));
}

void RenderThread::ExecuteWorks(IRHIDeviceContext* InDeviceContext, std::function<void()> InCompletionWork, bool bWaitPreviousWork)
{
	if (bWaitPreviousWork)
	{
		WaitForLastWorks();
	}

	std::unique_lock lock(_Thread.WorkerMtx);
	_WaitingWorks.CompletedWork = InCompletionWork;
	_ExecutingWorks.SwapExecute(InDeviceContext, _WaitingWorks);

	_Thread.WorkerPromise = std::promise<void>();
	_Thread.WorkerFuture = _Thread.WorkerPromise.get_future();
	_Thread.cvWorker.notify_one();
}

void RenderThread::WaitForLastWorks()
{
	if (_Thread.WorkerFuture.valid())
	{
		_Thread.WorkerFuture.wait();
	}
}

void RenderThread::OnPreGarbageCollect()
{
	if (_Thread.WorkerFuture.valid())
	{
		_Thread.WorkerFuture.get();
	}
}

void RenderThread::OnPostGarbageCollect()
{
}