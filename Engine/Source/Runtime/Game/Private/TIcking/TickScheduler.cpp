// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Ticking/TickScheduler.h"

GENERATE_BODY(STickScheduler);

STickScheduler::STickScheduler() : Super()
{
}

void STickScheduler::Tick(float InDetlaTime)
{
	using namespace std::chrono;

	for (auto& Task : Tasks)
	{
		auto& Info = Task.second;

		Info.ActualDelay -= InDetlaTime;
		if (Info.ActualDelay <= 0)
		{
			Info.Task();

			if (Info.bReliableCallCount)
			{
				Info.ActualDelay += Info.Delay;
			}
			else
			{
				Info.ActualDelay = Info.Delay;
			}
		}
	}
}

int64 STickScheduler::AddSchedule(const STickScheduler::TaskInfo& TaskInfo)
{
	TickTaskInstance InternalInfo;
	InternalInfo.Task = TaskInfo.Task;
	InternalInfo.Delay = TaskInfo.Delay;
	InternalInfo.ActualDelay = TaskInfo.InitDelay;
	InternalInfo.bReliableCallCount = TaskInfo.bReliableCallCount;

	int64 MyId = Id++;
	Tasks.emplace(MyId, InternalInfo);
	return MyId;
}

void STickScheduler::RemoveSchedule(int64 TaskId)
{
	Tasks.erase(TaskId);
}