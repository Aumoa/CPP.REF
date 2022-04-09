// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include <functional>
#include <map>

namespace libty::inline Core
{
	class CORE_API TickScheduler
	{
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
			std::function<bool()> Validator;
		};

		int64 Id = 0;
		std::map<size_t, TickTaskInstance> Tasks;

	public:
		TickScheduler();

		void Tick(float InDeltaTime);
		int64 AddSchedule(const TaskInfo& TaskInfo);
		int64 AddSchedule(std::function<bool()> InValidator, const TaskInfo& TaskInfo);
		void RemoveSchedule(int64 TaskId);
	};
}