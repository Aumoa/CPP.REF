// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "IO/IOContext.h"
#include "IO/IOSignalOverlapped.h"
#include "IO/IOActionOverlapped.h"
#include "Misc/PlatformMisc.h"

IOContext::IOContext()
{
	PlatformMisc::InitializeIOCompletionPort(&_iocp);
}

IOContext::~IOContext() noexcept
{
	PlatformMisc::DestroyIOCompletionPort(_iocp);
}

size_t IOContext::Poll()
{
	size_t cnt = 0;
	while (PollOne())
	{
		++cnt;
	}
	return cnt;
}

size_t IOContext::PollOne()
{
	if (_running == false)
	{
		return 0;
	}

	size_t transf = 0;
	if (IOCompletionOverlapped* overlap = PlatformMisc::GetQueuedCompletionStatus(_iocp, 0, &transf))
	{
		if (overlap->Complete(transf))
		{
			delete overlap;
		}
		
		return 1;
	}
	return 0;
}

size_t IOContext::Run()
{
	if (_running == false)
	{
		return 0;
	}

	size_t cnt = 0;
	++_workers;

	while (_running)
	{
		size_t transf = 0;
		IOCompletionOverlapped* overlap = PlatformMisc::GetQueuedCompletionStatus(_iocp, 0xFFFFFFFF, &transf);
		if (overlap)
		{
			if (overlap->Complete(transf))
			{
				delete overlap;
			}

			++cnt;
		}
	}

	--_workers;
	return cnt;
}

void IOContext::Stop()
{
	_running = false;
	size_t workers = _workers;
	for (size_t i = 0; i < workers; ++i)
	{
		PlatformMisc::PostQueuedCompletionStatus(_iocp, new IOSignalOverlapped(0), 0);
	}

	while (_workers)
	{
		for (size_t i = 0; i < 1024; ++i)
		{
			PlatformMisc::YieldProcessor();
		}
	}

	Poll();
}

void IOContext::BindHandle(void* nativeHandle)
{
	PlatformMisc::BindIOCompletionPort(&_iocp, nativeHandle);
}

bool IOContext::Post(std::function<void()> work)
{
	if (_running == false)
	{
		return false;
	}

	return PlatformMisc::PostQueuedCompletionStatus(_iocp, new IOActionOverlapped(std::move(work)), 0);
}