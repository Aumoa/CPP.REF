// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Threading/Task.h"
#include "Threading/ThreadGroup.h"
#include <boost/asio/io_service.hpp>
#include <boost/asio/deadline_timer.hpp>

namespace Threading::Tasks
{
	ThreadGroup ThreadPool(L"TaskWorker");
	boost::asio::io_service DeferredIO;
	std::atomic<size_t> DeferredIOCount;

	std::optional<bool> TaskSource<void>::bFallbackToDeferred;

	TaskSource<void>::TaskSource()
	{
	}

	TaskSource<void>::TaskSource(std::shared_ptr<SourceAwaiter> Awaiter)
		: Awaiter(std::move(Awaiter))
	{
	}

	void TaskSource<void>::FlushDeferred()
	{
		size_t Works = DeferredIOCount;
		size_t Consume = 0;
		for (size_t i = 0; i < Works; ++i)
		{
			Consume += DeferredIO.poll_one();
		}
		DeferredIOCount -= Consume;
	}

	void TaskSource<void>::Run(std::function<void()> Body)
	{
		ThreadPool.Run(Body);
	}

	void TaskSource<void>::Deferred(std::function<void()> Body)
	{
		// Increment IO count first. If does not consume deferred action actually, count will not decrement.
		// DeferredIOCount just check count that execute per frame.
		++DeferredIOCount;
		DeferredIO.post(Body);
	}

	void TaskSource<void>::Delay(std::chrono::milliseconds Timeout, std::function<void()> Body)
	{
		ThreadPool.Delay(Timeout, std::move(Body));
	}

	std::shared_ptr<Awaiter<void>> TaskSource<void>::CompletedAwaiter()
	{
		class FreezedAwaiter : public Tasks::Awaiter<void>
		{
		public:
			FreezedAwaiter()
			{
				SetValue();
			}

			virtual bool Cancel() override
			{
				checkf(false, L"Awaiter is freezed from CompletedTask.");
				return false;
			}
		};

		thread_local static std::shared_ptr<FreezedAwaiter> Awaiter = std::make_shared<FreezedAwaiter>();
		return Awaiter;
	}
}