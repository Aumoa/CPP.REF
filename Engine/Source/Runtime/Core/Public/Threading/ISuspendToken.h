// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <future>

class CORE_API ISuspendToken
{
public:
	virtual ~ISuspendToken() noexcept = default;

	virtual std::future<void> Suspend() = 0;
	virtual void Resume() = 0;
};