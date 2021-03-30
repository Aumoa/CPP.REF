// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Misc/TickScheduler.h"

#include "Diagnostics/ScopedCycleCounter.h"

using namespace std;

DEFINE_STATS_GROUP(TickScheduler);

TickScheduleTaskInfo::TickScheduleTaskInfo()
	: Delay(0ns)
	, InitDelay(0ns)
	, bReliableCallCount(false)
{

}

TickScheduler::TickScheduler() : Super()
{

}

TickScheduler::~TickScheduler()
{

}

void TickScheduler::Tick(Seconds deltaTime)
{
	for (auto& task : tasks)
	{
		auto& info = task.second;

		info.ActualDelay -= deltaTime;
		if (info.ActualDelay.Unit <= 0ns)
		{
			info.Task();

			if (info.bReliableCallCount)
			{
				info.ActualDelay += info.Delay;
			}
			else
			{
				info.ActualDelay = info.Delay;
			}
		}
	}
}

bool TickScheduler::AddSchedule(const TickScheduleTaskInfo& taskInfo)
{
	TickTaskInfo internalInfo;
	internalInfo.Task = taskInfo.Task;
	internalInfo.Delay = taskInfo.Delay;
	internalInfo.ActualDelay = taskInfo.InitDelay;
	internalInfo.bReliableCallCount = taskInfo.bReliableCallCount;

	return tasks.emplace(internalInfo.Task.GetHashCode(), internalInfo).second;
}

bool TickScheduler::RemoveSchedule(TFunction<void()> task)
{
	if (auto it = tasks.find(task.GetHashCode()); it != tasks.end())
	{
		tasks.erase(it);
		return true;
	}

	return false;
}