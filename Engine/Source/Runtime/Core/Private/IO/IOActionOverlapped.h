// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "IO/IOCompletionOverlapped.h"
#include <functional>

class CORE_API IOActionOverlapped : public IOCompletionOverlapped
{
	std::function<void()> _work;

public:
	IOActionOverlapped(std::function<void()> work)
		: _work(std::move(work))
	{
	}

	virtual bool Complete() override
	{
		_work();
		return true;
	}
};