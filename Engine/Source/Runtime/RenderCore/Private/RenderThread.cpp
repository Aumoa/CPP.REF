// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "RenderThread.h"
#include "Threading/Thread.h"
#include <mutex>
#include <condition_variable>
#include <boost/asio/io_service.hpp>

RenderThread* RenderThread::sInstance;

DECLARE_STAT_GROUP("RenderThread", STATGROUP_RenderThread);
DECLARE_CYCLE_STAT("ExecuteWorks", STAT_ExecuteWorks, STATGROUP_RenderThread);
DECLARE_CYCLE_STAT("  WaitPreviousComplete", STAT_WaitPreviousComplete, STATGROUP_RenderThread);
DECLARE_CYCLE_STAT("  ExecuteCurrentWorks", STAT_ExecuteCurrentWorks, STATGROUP_RenderThread);

struct RenderThread::Impl_t
{
	boost::asio::io_service IO;
	IRHIDeviceContext* Context = nullptr;

	std::function<void(IRHIDeviceContext*)> CompletionWork;
};

RenderThread::RenderThread()
{
	checkf(sInstance == nullptr, L"Singleton instance duplicated.");
	sInstance = this;

	Impl[0] = new Impl_t();
	Impl[1] = new Impl_t();

	bRunning = true;
	RemoteThread = Thread::CreateThread(L"[Render Thread]", std::bind(&RenderThread::Run, this));
}

RenderThread::~RenderThread()
{
	bRunning = false;
	Task<>::SetManual(Runner);
	RemoteThread->Join();

	delete Impl[0];
	delete Impl[1];
	sInstance = nullptr;
}

void RenderThread::EnqueueRenderThreadWork(std::function<void(IRHIDeviceContext*)> Work)
{
	size_t Index = BufferIndex % 2;
	Impl[Index]->IO.post([this, Index, Work = std::move(Work)]() mutable
	{
		Work(Impl[Index]->Context);
	});
}

Task<> RenderThread::ExecuteWorks(IRHIDeviceContext* InDeviceContext, std::function<void(IRHIDeviceContext*)> InCompletionWork)
{
	SCOPE_CYCLE_COUNTER(STAT_ExecuteWorks);

	size_t Index = BufferIndex++ % 2;
	auto Impl = this->Impl[Index];

	{
		SCOPE_CYCLE_COUNTER(STAT_WaitPreviousComplete);
		Completed.Wait();
		Completed.GetAwaiter()->Reset();
	}

	Impl->CompletionWork = InCompletionWork;
	Impl->Context = InDeviceContext;

	{
		SCOPE_CYCLE_COUNTER(STAT_ExecuteCurrentWorks);
		Task<>::SetManual(Runner);
		co_await Completed;
	}
}

bool RenderThread::InRenderThread()
{
	return Thread::GetCurrentThread()->GetThreadId() == Get()->RemoteThread->GetThreadId();
}

void RenderThread::Run()
{
	Thread* CurrentThread = Thread::GetCurrentThread();

	class RenderThreadSuspendToken : public SuspendToken
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
			Task<>::SetManual(Owner->Runner);
			return SuspendPromise.emplace().get_future();
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
	GC.AddSuspendToken(Token.get());

	while (bRunning)
	{
		Token->Join();

		auto This = this;
		size_t Index = BufferIndex % 2;
		auto Impl = this->Impl[Index];

		// Waiting for event 'Run'.
		Runner.Wait();
		Runner.GetAwaiter()->Reset();

		if (Impl->CompletionWork)
		{
			Impl->IO.run();
			Impl->CompletionWork(Impl->Context);
			Task<>::SetManual(Completed);
		}
		else
		{
			// Continue for GC without complete.
			continue;
		}
	}

	GC.RemoveSuspendToken(Token.get());
}