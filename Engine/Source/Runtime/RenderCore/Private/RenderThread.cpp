// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RenderThread.h"
#include "RenderContext.h"

using namespace ::libty;

SRenderThread* SRenderThread::sInstance;

DECLARE_STAT_GROUP("RenderThread", STATGROUP_RenderThread);
DECLARE_CYCLE_STAT("ExecuteWorks", STAT_ExecuteWorks, STATGROUP_RenderThread);
DECLARE_CYCLE_STAT("  WaitPreviousComplete", STAT_WaitPreviousComplete, STATGROUP_RenderThread);
DECLARE_CYCLE_STAT("  ExecuteCurrentWorks", STAT_ExecuteCurrentWorks, STATGROUP_RenderThread);

SRenderThread::SRenderThread()
{
	checkf(sInstance == nullptr, TEXT("Singleton instance duplicated."));
	sInstance = this;

	_running = true;
	_thread = Thread::CreateThread(TEXT("[Render Thread]"), std::bind(&SRenderThread::Run, this));
}

SRenderThread::~SRenderThread()
{
	if (_running)
	{
		Shutdown();
	}
}

void SRenderThread::EnqueueRenderThreadWork(SObject* object, RenderThreadWork work)
{
	_queuedWorks.emplace_back(Work
	{
		.Holder = object,
		.Body = work
	});
}

void SRenderThread::ExecuteWorks(SRenderContext* InRenderContext, RenderThreadWork InCompletionWork)
{
	check(InRenderContext);
	SCOPE_CYCLE_COUNTER(STAT_ExecuteWorks);

	std::unique_lock lock(_lock);
	_dequeued.Wait(lock, [this]() { return _rcontext == nullptr; });

	if (_exception)
	{
		std::rethrow_exception(_exception);
	}

	_queuedWorks.emplace_back(Work{ .Holder = this, .Body = std::move(InCompletionWork) });
	_rcontext = InRenderContext;

	_invoke.NotifyOne();
}

void SRenderThread::Shutdown()
{
	if (_running)
	{
		Task<> jtask = _thread->JoinAsync();

		std::unique_lock lock(_lock);
		_running = false;
		_invoke.NotifyAll();
		lock.unlock();

		jtask.GetResult();
		sInstance = nullptr;
	}
}

bool SRenderThread::InRenderThread()
{
	return Thread::GetCurrentThread()->GetThreadId() == Get()->_thread->GetThreadId();
}

void SRenderThread::Run()
{
	std::vector<Work> queuedWorks;
	SRenderContext* rcontext = nullptr;

	while (true)
	{
		// Waiting for event 'Run'.
		std::unique_lock lock(_lock);
		_invoke.Wait(lock, [this]() { return _rcontext != nullptr || !_running; });

		if (!_running)
		{
			break;
		}

		std::swap(queuedWorks, _queuedWorks);
		std::swap(rcontext, _rcontext);

		_dequeued.NotifyOne();
		lock.unlock();

		try
		{
			for (auto& work : queuedWorks)
			{
				if (work.Holder.IsValid())
				{
					work.Body(rcontext);
				}
			}
		}
		catch (const Exception&)
		{
			lock.lock();
			_exception = std::current_exception();
		}

		if (!lock.owns_lock())
		{
			lock.lock();
		}

		queuedWorks.clear();
		rcontext = nullptr;
	}
}