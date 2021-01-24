// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

struct GAME_API TickScheduleTaskInfo
{
	TFunction<void()> Task;
	Milliseconds Delay;
	Milliseconds InitDelay;
	bool bReliableCallCount : 1;

	TickScheduleTaskInfo();
};

class GAME_API TickScheduler : virtual public Object
{
public:
	using Super = Object;
	using This = TickScheduler;

private:
	struct TickTaskInfo
	{
		Milliseconds Delay;
		Milliseconds ActualDelay;
		bool bReliableCallCount : 1;

		TFunction<void()> Task;
	};

	std::map<size_t, TickTaskInfo> tasks;

public:
	TickScheduler();
	~TickScheduler();

	void Tick(Seconds deltaTime);

	bool AddSchedule(const TickScheduleTaskInfo& taskInfo);
	bool RemoveSchedule(TFunction<void()> task);
};