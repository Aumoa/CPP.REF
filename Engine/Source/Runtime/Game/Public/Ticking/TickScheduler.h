// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <functional>
#include <chrono>
#include <atomic>
#include <map>

class STickScheduler : implements SObject
{
	GENERATED_BODY(STickScheduler)

public:
	struct TaskInfo
	{
		std::function<void()> Task;
		float Delay = 0;
		float InitDelay = 0;
		bool bReliableCallCount : 1 = false;
	};

private:
	struct TickTaskInstance
	{
		float Delay = 0;
		float ActualDelay = 0;
		bool bReliableCallCount : 1 = false;

		std::function<void()> Task;
	};

	std::atomic<int64> _id = 0;
	std::map<size_t, TickTaskInstance> _tasks;

public:
	inline STickScheduler()
	{
	}

	inline void Tick(float deltaTime)
	{
		using namespace std::chrono;

		for (auto& task : _tasks)
		{
			auto& info = task.second;

			info.ActualDelay -= deltaTime;
			if (info.ActualDelay <= 0)
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

	inline int64 AddSchedule(const STickScheduler::TaskInfo& taskInfo)
	{
		TickTaskInstance internalInfo;
		internalInfo.Task = taskInfo.Task;
		internalInfo.Delay = taskInfo.Delay;
		internalInfo.ActualDelay = taskInfo.InitDelay;
		internalInfo.bReliableCallCount = taskInfo.bReliableCallCount;

		int64 id = _id++;
		_tasks.emplace(id, internalInfo);
		return id;
	}

	inline void RemoveSchedule(int64 taskId)
	{
		_tasks.erase(taskId);
	}
};