// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RenderThread.h"
#include "Threading/Thread.h"
#include "Threading/ThreadGroup.h"
#include "Threading/SuspendTokenCollection.h"
#include <mutex>
#include <condition_variable>

RenderThread* RenderThread::sInstance;

DECLARE_STAT_GROUP("RenderThread", STATGROUP_RenderThread);
DECLARE_CYCLE_STAT("ExecuteWorks", STAT_ExecuteWorks, STATGROUP_RenderThread);
DECLARE_CYCLE_STAT("  WaitPreviousComplete", STAT_WaitPreviousComplete, STATGROUP_RenderThread);
DECLARE_CYCLE_STAT("  ExecuteCurrentWorks", STAT_ExecuteCurrentWorks, STATGROUP_RenderThread);

RenderThread::RenderThread()
{
	checkf(sInstance == nullptr, L"Singleton instance duplicated.");
	sInstance = this;

	_running = true;
	_rthread = Thread::CreateThread(L"[Render Thread]", std::bind(&RenderThread::Run, this));
}

RenderThread::~RenderThread()
{
	_running = false;
	_rthread->Join();

	sInstance = nullptr;
}

void RenderThread::EnqueueRenderThreadWork(SObject* object, std::function<void(IRHICommandBuffer*)> work)
{
	_queuedWorks.emplace(Work
	{
		.Holder = object,
		.Body = work
	});
}

Task<> RenderThread::ExecuteWorks(IRHICommandBuffer* InDeviceContext, std::function<void(IRHICommandBuffer*)> InCompletionWork)
{
	SCOPE_CYCLE_COUNTER(STAT_ExecuteWorks);

	if (_taskCompletionSource.IsValid())
	{
		co_await _taskCompletionSource.GetTask();
	}

	{
		std::unique_lock lock(_lock);
		_taskCompletionSource = TaskCompletionSource<>::Create();
		_completion = InCompletionWork;
	}

	_invoke.notify_one();

	{
		SCOPE_CYCLE_COUNTER(STAT_ExecuteCurrentWorks);
		co_await _taskCompletionSource.GetTask();
	}
}

bool RenderThread::InRenderThread()
{
	return Thread::GetCurrentThread()->GetThreadId() == Get()->_rthread->GetThreadId();
}

void RenderThread::Run()
{
	Thread* CurrentThread = Thread::GetCurrentThread();

	class RenderThreadSuspendToken : public ISuspendToken
	{
		RenderThread* Owner;
		Thread* CurrentThread;

		std::optional<std::promise<void>> SuspendPromise;

	public:
		RenderThreadSuspendToken(RenderThread* Owner, Thread* CurrentThread)
			: Owner(Owner)
			, CurrentThread(CurrentThread)
		{
		}

		virtual std::future<void> Suspend() override
		{
			checkf(!SuspendPromise.has_value(), L"Thread already wait for suspend.");
			SuspendPromise.emplace();

			Owner->_taskCompletionSource.GetTask().Wait();
			Owner->_invoke.notify_one();

			return SuspendPromise->get_future();
		}

		virtual void Resume() override
		{
			if (SuspendPromise.has_value())
			{
				SuspendPromise.reset();
				CurrentThread->ResumeThread();
			}
		}

		void Join()
		{
			if (!SuspendPromise.has_value())
			{
				return;
			}

			SuspendPromise->set_value();
			CurrentThread->SuspendThread();
		}
	};

	std::unique_ptr Token = std::make_unique<RenderThreadSuspendToken>(this, CurrentThread);
	SuspendTokenCollection::Add(Token.get());

	while (_running)
	{
		Token->Join();

		// Waiting for event 'Run'.
		std::unique_lock lock(_lock);
		_invoke.wait(lock);

		std::queue<Work> works;
		std::swap(works, _queuedWorks);

		IRHICommandBuffer* deviceContext = _deviceContext;
		std::function<void(IRHICommandBuffer*)> completion = std::move(_completion);

		lock.unlock();

		while (!works.empty())
		{
			Work& work = works.front();
			if (work.Holder.IsValid())
			{
				work.Body(deviceContext);
			}
			works.pop();
		}

		if (completion)
		{
			completion(deviceContext);
		}

		_taskCompletionSource.SetResult();
	}

	SuspendTokenCollection::Remove(Token.get());
}