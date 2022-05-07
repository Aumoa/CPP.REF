// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RenderThread.h"

using namespace ::libty;

SRenderThread* SRenderThread::sInstance;

DECLARE_STAT_GROUP("RenderThread", STATGROUP_RenderThread);
DECLARE_CYCLE_STAT("ExecuteWorks", STAT_ExecuteWorks, STATGROUP_RenderThread);
DECLARE_CYCLE_STAT("  WaitPreviousComplete", STAT_WaitPreviousComplete, STATGROUP_RenderThread);
DECLARE_CYCLE_STAT("  ExecuteCurrentWorks", STAT_ExecuteCurrentWorks, STATGROUP_RenderThread);

SRenderThread::SRenderThread()
{
	checkf(sInstance == nullptr, L"Singleton instance duplicated.");
	sInstance = this;

	_running = true;
	_thread = Thread::CreateThread(L"[Render Thread]", std::bind(&SRenderThread::Run, this));
}

SRenderThread::~SRenderThread()
{
	std::unique_lock lock(_lock);
	_running = false;
	_taskCompletionSource = TaskCompletionSource<>::Create();
	_invoke.NotifyAll();
	lock.unlock();

	_thread->Join();
	sInstance = nullptr;
}

void SRenderThread::EnqueueRenderThreadWork(SObject* object, std::function<void(IRHIGraphicsCommandList*)> work)
{
	_queuedWorks.emplace(Work
	{
		.Holder = object,
		.Body = work
	});
}

Task<> SRenderThread::ExecuteWorks(IRHIGraphicsCommandList* InDeviceContext, std::function<void(IRHIGraphicsCommandList*)> InCompletionWork)
{
	SCOPE_CYCLE_COUNTER(STAT_ExecuteWorks);

	if (_taskCompletionSource.IsValid())
	{
		_taskCompletionSource.GetTask().Wait();
	}

	{
		std::unique_lock lock(_lock);
		_taskCompletionSource = TaskCompletionSource<>::Create();
		_completion = InCompletionWork;
	}

	_invoke.NotifyOne();

	{
		SCOPE_CYCLE_COUNTER(STAT_ExecuteCurrentWorks);
		co_await _taskCompletionSource.GetTask();
	}
}

bool SRenderThread::InRenderThread()
{
	return Thread::GetCurrentThread()->GetThreadId() == Get()->_thread->GetThreadId();
}

void SRenderThread::Run()
{
	Thread* CurrentThread = Thread::GetCurrentThread();

	class RenderThreadSuspendToken : public ISuspendToken
	{
		SRenderThread* Owner;
		Thread* CurrentThread;

		std::optional<std::promise<void>> SuspendPromise;

	public:
		RenderThreadSuspendToken(SRenderThread* Owner, Thread* CurrentThread)
			: Owner(Owner)
			, CurrentThread(CurrentThread)
		{
		}

		virtual std::future<void> Suspend() override
		{
			checkf(!SuspendPromise.has_value(), L"Thread already wait for suspend.");
			SuspendPromise.emplace();

			Owner->_taskCompletionSource.GetTask().Wait();
			Owner->_invoke.NotifyOne();

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
		_invoke.Wait(lock);

		std::queue<Work> works;
		std::swap(works, _queuedWorks);

		TaskCompletionSource<> completionSrc = _taskCompletionSource;
		IRHIGraphicsCommandList* deviceContext = _deviceContext;
		std::function<void(IRHIGraphicsCommandList*)> completion = std::move(_completion);

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

		lock.lock();
		completionSrc.SetResult();
	}

	SuspendTokenCollection::Remove(Token.get());
}