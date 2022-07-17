// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/RHIRenderThread.h"
#include "RHIRenderThread.gen.cpp"

RHIRenderThread::RHIRenderThread()
{
	_running = true;
	_thread = Thread::CreateThread(TEXT("[RHI]"), std::bind(&RHIRenderThread::Worker, this));
}

void RHIRenderThread::StopThread()
{
	std::unique_lock lock(_mutex);
	_running = false;
	_cv.NotifyOne();
}

void RHIRenderThread::Worker()
{
	while (_running)
	{
		std::unique_lock lock(_mutex);
		_cv.Wait(lock, [this]() { return !_running; });
	}
}