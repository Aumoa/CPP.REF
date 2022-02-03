// Copyright 2020-2022 Aumoa.lib. All right reserved.

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

void RenderThread::EnqueueRenderThreadWork(SObject* Object, std::function<void(IRHICommandBuffer*)> Work)
{
	size_t Index = BufferIndex % 2;
	Impl[Index]->IO.post([this, Work = std::move(Work), bNullHolder = Object == nullptr, Holder = WeakPtr(Object)]() mutable
	{
		if (bNullHolder || Holder.IsValid())
		{
			Work(Context);
		}
	});
}

Task<> RenderThread::ExecuteWorks(IRHICommandBuffer* InDeviceContext, std::function<void(IRHICommandBuffer*)> InCompletionWork)
{
	SCOPE_CYCLE_COUNTER(STAT_ExecuteWorks);

	size_t Index = BufferIndex++ % 2;
	auto Impl = this->Impl[Index];

	{
		SCOPE_CYCLE_COUNTER(STAT_WaitPreviousComplete);
		Completed.Wait();
		Completed.GetAwaiter()->Reset();
	}

	CompletionWork = InCompletionWork;
	Context = InDeviceContext;
	RenderBufferIndex = Index;

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
			SuspendPromise.emplace();

			Owner->Completed.Wait();
			Task<>::SetManual(Owner->Runner);

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
	GC.AddSuspendToken(Token.get());

	while (bRunning)
	{
		Token->Join();

		// Waiting for event 'Run'.
		Runner.Wait();
		Runner.GetAwaiter()->Reset();

		size_t Index = RenderBufferIndex;
		auto Impl = this->Impl[Index];

		if (CompletionWork)
		{
			Impl->IO.run();
			CompletionWork(Context);

			Context = nullptr;
			CompletionWork = nullptr;

			Task<>::SetManual(Completed);
		}
		else
		{
			// Continue for GC without complete.
			Impl->IO.reset();
			continue;
		}
	}

	GC.RemoveSuspendToken(Token.get());
}