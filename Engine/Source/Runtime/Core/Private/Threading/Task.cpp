// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Threading/Tasks/Task.h"
#include "Threading/ThreadGroup.h"
#include <optional>

namespace Threading::Tasks
{
	static std::optional<ThreadGroup> sTaskWorker;
}

template<>
void Task<>::Initialize()
{
	Threading::Tasks::sTaskWorker.emplace(TEXT("TaskWorker"));
}

template<>
void Task<>::Shutdown()
{
	Threading::Tasks::sTaskWorker.reset();
}

template<>
void Task<>::Run_thread(std::function<void()> body)
{
	Threading::Tasks::sTaskWorker->Run(std::move(body));
}

template<>
void Task<>::Delay_thread(std::chrono::milliseconds delay, std::function<void()> body)
{
	Threading::Tasks::sTaskWorker->Delay(delay, std::move(body));
}