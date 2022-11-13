// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "IO/IOCompletionOverlapped.h"

class IOFileOverlapped : public IOCompletionOverlapped
{
	TaskCompletionSource<size_t> _resolver;

public:
	IOFileOverlapped(TaskCompletionSource<size_t> resolver)
		: _resolver(std::move(resolver))
	{
	}

	virtual bool Complete(size_t resolved) override
	{
		_resolver.SetResult(resolved);
		return true;
	}

	virtual bool Failed(std::exception_ptr ptr) override
	{
		_resolver.SetException(std::move(ptr));
		return true;
	}
};