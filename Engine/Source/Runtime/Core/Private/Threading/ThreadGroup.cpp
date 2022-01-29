// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Threading/ThreadGroup.h"
#include <thread>
#include <boost/asio/io_service.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio/deadline_timer.hpp>
#include "Threading/Thread.h"

struct ThreadGroup::Impl
{
	boost::asio::io_service IO;
	std::optional<boost::asio::io_service::work> CoreWork;
	boost::thread_group Threads;
};

ThreadGroup::ThreadGroup(std::wstring_view GroupName, size_t NumThreads)
	: _Impl(new Impl())
	, GroupName(GroupName)
{
	if (NumThreads == 0)
	{
		NumThreads = (size_t)std::thread::hardware_concurrency();
	}

	_Impl->CoreWork.emplace(_Impl->IO);

	for (size_t i = 0; i < NumThreads; ++i)
	{
		_Impl->Threads.create_thread(std::bind(&ThreadGroup::Worker, this, i));
	}
}

ThreadGroup::~ThreadGroup()
{
	_Impl->IO.stop();
	_Impl->Threads.join_all();

	delete _Impl;
}

void ThreadGroup::Run(std::function<void()> Body)
{
	_Impl->IO.post(Body);
}

void ThreadGroup::Delay(std::chrono::milliseconds Timeout, std::function<void()> Body)
{
	boost::asio::deadline_timer Timer(_Impl->IO);
	Timer.expires_from_now(boost::posix_time::millisec(Timeout.count()));
	Timer.async_wait([Body = std::move(Body)](boost::system::error_code Error)
	{
		if (Error.value() != 0)
		{
			Body();
		}
	});
}

void ThreadGroup::Worker(size_t Index)
{
	Thread* MyThread = Thread::GetCurrentThread();
	std::wstring ThreadWorkerName = std::format(L"[{} #{}]", GroupName, Index);
	MyThread->SetFriendlyName(ThreadWorkerName);

	// Blocking the thread for all times.
	_Impl->IO.run();
}