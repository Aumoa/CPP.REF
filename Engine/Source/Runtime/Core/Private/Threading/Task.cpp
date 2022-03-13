// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Threading/Task.h"
#include "Threading/ThreadGroup.h"

namespace libty::Threading::Tasks::Impl
{
	static ThreadGroup group(L"TaskWorker");

	void Run(std::function<void()> body)
	{
		group.Run(std::move(body));
	}

	void Delay(std::chrono::milliseconds delay, std::function<void()> body)
	{
		group.Delay(delay, body);
	}
}