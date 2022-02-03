// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Threading/ThreadGroup.h"
#include "Threading/SuspendToken.h"
#include "Threading/Thread.h"
#include <future>
#include <boost/asio/io_service.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/spawn.hpp>

struct ThreadGroup::Impl
{
	boost::asio::io_service IO;
	std::optional<boost::asio::io_service::work> CoreWork;
	boost::thread_group Threads;
	volatile bool bRunning = true;
};

class ThreadGroup::ThreadGroupSuspendToken : public SuspendToken
{
	Impl* _Impl;
	std::optional<std::promise<void>> SuspendPromise;

	std::vector<Thread*> SuspendThreads;
	std::atomic<size_t> SuspendThreadsIdx;

public:
	ThreadGroupSuspendToken(Impl* _Impl, size_t NumThreads)
		: _Impl(_Impl)
		, SuspendThreads(NumThreads)
	{
	}

	virtual ~ThreadGroupSuspendToken() noexcept override
	{
	}

	virtual std::future<void> Suspend() override
	{
		checkf(!SuspendPromise.has_value(), L"ThreadGroup already wait for suspend.");
		SuspendPromise.emplace();

		SuspendThreadsIdx = 0;
		_Impl->IO.stop();

		return SuspendPromise->get_future();
	}

	virtual void Resume() override
	{
		SuspendPromise.reset();
		_Impl->IO.restart();

		for (auto& SuspendThread : SuspendThreads)
		{
			SuspendThread->ResumeThread();
		}
	}

	void Join(Thread* MyThread)
	{
		if (SuspendPromise.has_value())
		{
			size_t Myidx = SuspendThreadsIdx++;
			SuspendThreads[Myidx] = MyThread;

			if (Myidx == SuspendThreads.size() - 1)
			{
				SuspendPromise->set_value();
			}

			MyThread->SuspendThread();
		}
	}
};

ThreadGroup::ThreadGroup(std::wstring_view GroupName, size_t NumThreads)
	: _Impl(new Impl())
	, GroupName(GroupName)
{
	if (NumThreads == 0)
	{
		NumThreads = (size_t)std::thread::hardware_concurrency();
	}

	SuspendToken = new ThreadGroupSuspendToken(_Impl, NumThreads);
	GC.AddSuspendToken(SuspendToken);
	_Impl->CoreWork.emplace(_Impl->IO);

	for (size_t i = 0; i < NumThreads; ++i)
	{
		_Impl->Threads.create_thread(std::bind(&ThreadGroup::Worker, this, i));
	}
}

ThreadGroup::~ThreadGroup()
{
	GC.RemoveSuspendToken(SuspendToken);

	_Impl->CoreWork.reset();
	_Impl->bRunning = false;
	_Impl->IO.stop();
	_Impl->Threads.join_all();

	delete SuspendToken;
	delete _Impl;
}

void ThreadGroup::Run(std::function<void()> Body)
{
	_Impl->IO.post(Body);
}

void ThreadGroup::Delay(std::chrono::milliseconds Timeout, std::function<void()> Body)
{
	boost::asio::spawn(_Impl->IO, [this, Body = std::move(Body), Timeout](boost::asio::yield_context Coroutine) mutable
	{
		boost::asio::deadline_timer Timer(boost::asio::get_associated_executor(Coroutine));
		Timer.expires_from_now(boost::posix_time::millisec(Timeout.count()));
		Timer.async_wait(Coroutine);

		Run(std::move(Body));
	});
}

void ThreadGroup::Worker(size_t Index)
{
	Thread* MyThread = Thread::GetCurrentThread();

	std::wstring ThreadWorkerName = std::format(L"[{} #{}]", GroupName, Index);
	MyThread->SetFriendlyName(ThreadWorkerName);

	// Blocking the thread for all times.
	while (_Impl->bRunning)
	{
		SuspendToken->Join(MyThread);
		_Impl->IO.run();
	}
}