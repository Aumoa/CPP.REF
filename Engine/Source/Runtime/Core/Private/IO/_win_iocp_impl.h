// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_WINDOWS

#include "IO/IOContext.h"
#include "Misc/WindowsPlatformMisc.h"

class IOContext::_iocp_impl
{
private:
	HANDLE _iocp = NULL;

public:
	_iocp_impl(size_t threadsHint = -1);
	~_iocp_impl() noexcept;

	bool poll_one();
	void bind_handle(void* handle);
	void post(std::function<void(size_t, int32)> work);
};

#endif