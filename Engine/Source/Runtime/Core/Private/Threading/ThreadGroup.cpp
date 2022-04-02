// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Threading/ThreadGroup.h"
#include "Threading/Thread.h"
#include "Threading/ISuspendToken.h"
#include "Threading/SuspendTokenCollection.h"
#include "Misc/Exceptions.h"
#include "Misc/TickCalc.h"
#include "Misc/String.h"
#include <future>

using namespace libty;

class ThreadGroup::ThreadGroupSuspendToken : public ISuspendToken
{
	ThreadGroup* _source;
	std::optional<std::promise<void>> _promise;

	std::vector<Thread*> _threads;
	std::atomic<size_t> _threadsIdx;

public:
	ThreadGroupSuspendToken(ThreadGroup* source, size_t NumThreads)
		: _source(source)
		, _threads(NumThreads)
	{
	}

	virtual ~ThreadGroupSuspendToken() noexcept override
	{
	}

	virtual std::future<void> Suspend() override
	{
		if (_promise.has_value())
		{
			throw invalid_operation("ThreadGroup already wait for suspend.");
		}

		_threadsIdx = 0;
		_promise.emplace();
		_source->_immQueue.cv.notify_all();

		return _promise->get_future();
	}

	virtual void Resume() override
	{
		_promise.reset();

		for (auto& SuspendThread : _threads)
		{
			SuspendThread->ResumeThread();
		}
	}

	void Join(Thread* MyThread)
	{
		if (_promise.has_value())
		{
			size_t Myidx = _threadsIdx++;
			_threads[Myidx] = MyThread;

			if (Myidx == _threads.size() - 1)
			{
				_promise->set_value();
			}

			MyThread->SuspendThread();
		}
	}
};

ThreadGroup::ThreadGroup(std::wstring_view groupName, size_t numThreads)
	: _groupName(groupName)
{
	if (numThreads == 0)
	{
		numThreads = (size_t)std::thread::hardware_concurrency();
	}

	_suspendToken = new ThreadGroupSuspendToken(this, numThreads);
	SuspendTokenCollection::Add(_suspendToken);

	for (size_t i = 0; i < numThreads; ++i)
	{
		_threads.emplace_back(std::jthread(std::bind(&ThreadGroup::Worker, this, i, std::placeholders::_1)));
	}

	_threads.emplace_back(std::jthread(std::bind(&ThreadGroup::Timer, this, std::placeholders::_1)));
}

ThreadGroup::~ThreadGroup() noexcept
{
	SuspendTokenCollection::Remove(_suspendToken);

	for (auto& Thread : _threads)
	{
		if (Thread.joinable())
		{
			Thread.request_stop();
			Thread.join();
		}
	}
}

void ThreadGroup::Run(std::function<void()> body)
{
	std::unique_lock lock(_immQueue.lock);
	_immQueue.enqueue(ImmediateWork{ .Body = body });
}

void ThreadGroup::Delay(std::chrono::milliseconds timeout, std::function<void()> body)
{
	std::unique_lock lock(_delQueue.lock);
	_delQueue.enqueue(DelayedWork{ .StartsWith = clock::now() + timeout, .Body = body });
}

void ThreadGroup::Worker(size_t index, std::stop_token cancellationToken)
{
	Thread* mythread = Thread::GetCurrentThread();
	mythread->SetFriendlyName(String::Format(L"[{} #{}]", _groupName, index));

	TickCalc<> timer;

	// Blocking the thread for all times.
	while (!cancellationToken.stop_requested())
	{
		auto now = clock::now();
		_suspendToken->Join(mythread);

		std::unique_lock lock(_immQueue.lock);
		size_t nonExpired = 0;

		while (_immQueue.queue.size() > nonExpired)
		{
			auto front = _immQueue.dequeue();
			lock.unlock();
			front.Body();
			_suspendToken->Join(mythread);
			lock.lock();
		}

		_immQueue.cv.wait(lock);
	}
}

void ThreadGroup::Timer(std::stop_token cancellationToken)
{
	Thread* mythread = Thread::GetCurrentThread();
	mythread->SetFriendlyName(String::Format(L"[{} #Timer]", _groupName));

	TickCalc<> timer;

	// Blocking the thread for all times.
	while (!cancellationToken.stop_requested())
	{
		std::unique_lock lock(_delQueue.lock);
		size_t nonExpired = 0;
		std::optional<clock::time_point> wait;

		while (_delQueue.queue.size() > nonExpired)
		{
			auto now = clock::now();

			auto front = _delQueue.dequeue();
			if (front.StartsWith <= now)
			{
				lock.unlock();
				front.Body();
				lock.lock();
				continue;
			}

			if (wait.has_value())
			{
				if (*wait > now)
				{
					wait = now;
				}
			}
			else
			{
				wait = now;
			}

			_delQueue.enqueue(std::move(front));
			++nonExpired;
		}

		if (wait.has_value())
		{
			_delQueue.cv.wait_until(lock, *wait);
		}
		else
		{
			_delQueue.cv.wait(lock);
		}
	}
}