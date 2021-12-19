// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <functional>
#include <chrono>
#include <atomic>
#include <map>

class GAME_API STickScheduler : implements SObject
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

	std::atomic<int64> Id = 0;
	std::map<size_t, TickTaskInstance> Tasks;

public:
	STickScheduler();

	void Tick(float InDeltaTime);
	int64 AddSchedule(const STickScheduler::TaskInfo& TaskInfo);
	void RemoveSchedule(int64 TaskId);
};