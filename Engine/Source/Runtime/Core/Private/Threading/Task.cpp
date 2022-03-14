// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Threading/Task.h"
#include "Threading/ThreadGroup.h"

static ThreadGroup group(L"TaskWorker");

template<>
void Task<>::RunImpl(std::function<void()> body)
{
	group.Run(std::move(body));
}

template<>
void Task<>::DelayImpl(std::chrono::milliseconds delay, std::function<void()> body)
{
	group.Delay(delay, body);
}