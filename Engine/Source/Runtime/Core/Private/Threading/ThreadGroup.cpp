// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Threading/ThreadGroup.h"
#include "Threading/Thread.h"
#include "Exceptions/InvalidOperationException.h"
#include "Misc/String.h"

ThreadGroup::ThreadGroup(const String& groupName, size_t numThreads)
	: _groupName(groupName)
{
	if (numThreads == 0)
	{
		numThreads = (size_t)std::thread::hardware_concurrency();
	}

	for (size_t i = 0; i < numThreads; ++i)
	{
		_threads.emplace_back(std::jthread(std::bind(&ThreadGroup::Worker, this, i, std::placeholders::_1)));
	}

	_threads.emplace_back(std::jthread(std::bind(&ThreadGroup::Timer, this, std::placeholders::_1)));
}

ThreadGroup::~ThreadGroup() noexcept
{
	// Unlock delay queue first! When timer working, call Run() function to queue works to work thread.
	std::unique_lock lock2(_delQueue.lock);
	std::unique_lock lock1(_immQueue.lock);

	for (auto& thread : _threads)
	{
		if (thread.joinable())
		{
			thread.request_stop();
		}
	}

	_delQueue.cv.NotifyAll();
	_immQueue.cv.NotifyAll();

	lock1.unlock();
	lock2.unlock();

	for (auto& thread : _threads)
	{
		if (thread.joinable())
		{
			thread.join();
		}
	}
}

void ThreadGroup::Run(std::function<void()> body)
{
	std::unique_lock lock(_immQueue.lock);
	_immQueue.enqueue(0, std::move(body));
}

void ThreadGroup::Delay(std::chrono::milliseconds timeout, std::function<void()> body)
{
	auto startsWith = clock::now() + timeout;
	std::unique_lock lock(_delQueue.lock);
	_delQueue.enqueue(std::move(startsWith), std::move(body));
}

void ThreadGroup::Worker(size_t index, std::stop_token cancellationToken)
{
	Thread mythread = Thread::GetCurrentThread();
	mythread.SetFriendlyName(String::Format(TEXT("[{} #{}]"), _groupName, index));

	// Blocking the thread for all times.
	while (!cancellationToken.stop_requested())
	{
		std::unique_lock lock(_immQueue.lock);
		size_t nonExpired = 0;

		while (_immQueue.queue.size() > nonExpired)
		{
			auto front = _immQueue.dequeue();
			lock.unlock();
			front();
			lock.lock();
		}

		_immQueue.cv.Wait(lock, [&]
		{
			return _immQueue.queue.size() || cancellationToken.stop_requested();
		});
	}
}

void ThreadGroup::Timer(std::stop_token cancellationToken)
{
	Thread mythread = Thread::GetCurrentThread();
	mythread.SetFriendlyName(String::Format(TEXT("[{} #Timer]"), _groupName));

	// Blocking the thread for all times.
	while (!cancellationToken.stop_requested())
	{
		std::unique_lock lock(_delQueue.lock);
		std::optional<clock::time_point> wait;

		while (_delQueue.queue.size() > 0)
		{
			auto now = clock::now();

			auto& priority = _delQueue.priority();
			if (priority <= now)
			{
				Run(_delQueue.dequeue());
			}
			else
			{
				wait = priority;
				break;
			}
		}

		if (wait.has_value())
		{
			_delQueue.cv.WaitUntil(lock, *wait);
		}
		else
		{
			_delQueue.cv.Wait(lock);
		}
	}
}