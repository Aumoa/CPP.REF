// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:TickScheduler;

import SC.Runtime.Core;
import std.core;

using namespace std;
using namespace std::chrono;

export struct TickScheduleTaskInfo
{
	function<void()> Task;
	duration<float> Delay = 0ns;
	duration<float> InitDelay = 0ns;
	bool bReliableCallCount : 1 = false;
};

export class TickScheduler : virtual public Object
{
public:
	using Super = Object;
	using This = TickScheduler;

private:
	struct TickTaskInfo
	{
		duration<float> Delay = 0ns;
		duration<float> ActualDelay = 0ns;
		bool bReliableCallCount : 1 = false;

		function<void()> Task;
	};

	atomic<int64> _id = 0;
	map<size_t, TickTaskInfo> _tasks;

public:
	TickScheduler()
	{
	}

	void Tick(duration<float> deltaTime)
	{
		for (auto& task : _tasks)
		{
			auto& info = task.second;

			info.ActualDelay -= deltaTime;
			if (info.ActualDelay <= 0ns)
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

	int64 AddSchedule(const TickScheduleTaskInfo& taskInfo)
	{
		TickTaskInfo internalInfo;
		internalInfo.Task = taskInfo.Task;
		internalInfo.Delay = taskInfo.Delay;
		internalInfo.ActualDelay = taskInfo.InitDelay;
		internalInfo.bReliableCallCount = taskInfo.bReliableCallCount;

		int64 id = _id++;
		_tasks.emplace(id, internalInfo);
		return id;
	}

	void RemoveSchedule(int64 taskId)
	{
		_tasks.erase(taskId);
	}
};