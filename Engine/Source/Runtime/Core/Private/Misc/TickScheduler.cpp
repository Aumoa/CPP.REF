// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Misc/TickScheduler.h"

TickScheduler::TickScheduler()
{
}

void TickScheduler::Tick(float InDetlaTime)
{
	using namespace std::chrono;

	std::vector<std::map<size_t, TickTaskInstance>::iterator> CompactList;
	for (auto It = Tasks.begin(); It != Tasks.end(); ++It)
	{
		auto& Instance = It->second;

		if (Instance.Validator && !Instance.Validator())
		{
			CompactList.emplace_back(It);
			continue;
		}

		Instance.ActualDelay -= InDetlaTime;
		if (Instance.ActualDelay <= 0)
		{
			Instance.Task();

			if (Instance.bReliableCallCount)
			{
				Instance.ActualDelay += Instance.Delay;
			}
			else
			{
				Instance.ActualDelay = Instance.Delay;
			}
		}
	}

	for (auto& It : CompactList)
	{
		Tasks.erase(It);
	}
}

int64 TickScheduler::AddSchedule(const TaskInfo& TaskInfo)
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

int64 TickScheduler::AddSchedule(SObject* InValidator, const TaskInfo& TaskInfo)
{
	TickTaskInstance InternalInfo;
	InternalInfo.Validator = [Holder = WeakPtr(InValidator)]{ return Holder.IsValid(); };
	InternalInfo.Task = TaskInfo.Task;
	InternalInfo.Delay = TaskInfo.Delay;
	InternalInfo.ActualDelay = TaskInfo.InitDelay;
	InternalInfo.bReliableCallCount = TaskInfo.bReliableCallCount;

	int64 MyId = Id++;
	Tasks.emplace(MyId, InternalInfo);
	return MyId;
}

void TickScheduler::RemoveSchedule(int64 TaskId)
{
	Tasks.erase(TaskId);
}