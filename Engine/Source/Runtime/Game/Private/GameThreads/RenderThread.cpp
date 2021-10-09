// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GameThreads/RenderThread.h"
#include "Threading/EventHandle.h"

int64 RenderThread::_threadId = 0;
std::thread RenderThread::_thread;
std::map<size_t, std::function<void()>> RenderThread::_works;
std::map<size_t, std::function<void()>> RenderThread::_pendingWorks;
std::mutex RenderThread::_locker;
std::shared_ptr<SEventHandle> RenderThread::_executeEvent;
std::atomic<bool> RenderThread::_bSwitch;

void RenderThread::Init()
{
	_thread = std::thread(Worker);
	_executeEvent = std::make_shared<SEventHandle>();
	_bSwitch = true;
}

void RenderThread::Shutdown()
{
	std::unique_lock lock(_locker);
	_bSwitch = false;
	_executeEvent->Set();
	_thread.join();
}

void RenderThread::EnqueueRenderThreadWork(size_t workingHash, std::function<void()> work)
{
	_works.emplace(workingHash, work);
}

void RenderThread::ExecuteWorks()
{
	std::unique_lock lock(_locker);
	std::swap(_pendingWorks, _works);
	_works.clear();
	_executeEvent->Set();
}

void RenderThread::InitThreadId()
{
	_threadId = SThread::GetCurrentThread()->GetThreadId();
}

void RenderThread::Worker()
{
	InitThreadId();

	while (_bSwitch)
	{
		_executeEvent->Wait();

		std::unique_lock lock(_locker);
		for (auto& workPair : _pendingWorks)
		{
			workPair.second();
		}
	}
}