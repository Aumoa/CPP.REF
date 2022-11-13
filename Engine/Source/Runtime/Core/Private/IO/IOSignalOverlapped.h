// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "IO/IOCompletionOverlapped.h"

class IOSignalOverlapped : public IOCompletionOverlapped
{
	int32 _signal = 0;

public:
	IOSignalOverlapped(int32 signal)
		: _signal(signal)
	{
	}

	int32 GetSignal() const noexcept { return _signal; }
	virtual bool Complete(size_t) override { return true; }
	virtual bool Failed(std::exception_ptr) override { return true; }
};