// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Threading/Tasks/Task.h"
#include "Threading/ThreadGroup.h"
#include <optional>

namespace Threading::Tasks
{
	static std::optional<ThreadGroup> sTaskWorker;
}

template<>
void Task<>::_Initialize()
{
	Threading::Tasks::sTaskWorker.emplace(TEXT("TaskWorker"));
}

template<>
void Task<>::_Shutdown()
{
	Threading::Tasks::sTaskWorker.reset();
}

template<>
void Task<>::_Run_thread(std::function<void()> body)
{
	Threading::Tasks::sTaskWorker->Run(std::move(body));
}

template<>
void Task<>::_Delay_thread(std::chrono::milliseconds delay, std::function<void()> body)
{
	Threading::Tasks::sTaskWorker->Delay(delay, std::move(body));
}