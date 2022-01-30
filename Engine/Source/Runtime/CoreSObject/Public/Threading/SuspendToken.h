// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <future>

class CORESOBJECT_API SuspendToken
{
public:
	SuspendToken();
	virtual ~SuspendToken() noexcept;

	virtual std::future<void> Suspend() = 0;
	virtual void Resume() = 0;
};