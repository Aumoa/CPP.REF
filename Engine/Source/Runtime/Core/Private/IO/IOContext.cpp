// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "IO/IOContext.h"
#include "IO/IOCompletionOverlapped.h"
#include "Misc/PlatformMisc.h"
#include "Exceptions/SystemException.h"
#include "_win_iocp_impl.h"

IOContext::IOContext()
	: _impl(std::make_unique<_iocp_impl>())
{
}

IOContext::~IOContext() noexcept
{
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
	if (_impl)
	{
		return _impl->poll_one() ? 1 : 0;
	}
	else
	{
		return 0;
	}
}

size_t IOContext::Run()
{
	if (_impl == nullptr)
	{
		return 0;
	}

	size_t cnt = 0;
	++_workers;

	while (_impl)
	{
		cnt += _impl->poll_one();
	}

	--_workers;
	return cnt;
}

void IOContext::Stop()
{
	size_t workers = _workers;
	for (size_t i = 0; i < workers; ++i)
	{
		_impl->post([this](size_t, int32) { --_workers; });
	}

	while (_workers)
	{
		for (size_t i = 0; i < 1024; ++i)
		{
			PlatformMisc::YieldProcessor();
		}
	}

	Poll();
	_impl.reset();
}

void IOContext::BindHandle(void* nativeHandle)
{
	_impl->bind_handle(nativeHandle);
}

bool IOContext::Post(std::function<void(size_t, int32)> work)
{
	if (_impl)
	{
		_impl->post(std::move(work));
		return true;
	}
	return false;
}