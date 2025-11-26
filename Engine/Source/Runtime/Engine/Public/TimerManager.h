// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Diagnostics/PerformanceTimer.h"

namespace Ayla
{
	class ENGINE_API TimerManager
	{
	public:
		/// <summary>
		/// Defines how timer intervals are calculated.
		/// </summary>
		enum class IntervalMode
		{
			/// <summary>
			/// Fixed interval mode - executes at absolute time points from the start.
			/// Prevents drift by aligning to fixed schedule (e.g., 00:01.000, 00:02.000, 00:03.000).
			/// Compensates for execution delays to maintain precise timing.
			/// </summary>
			Absolute,

			/// <summary>
			/// Relative interval mode - executes after the specified interval from the last execution.
			/// May accumulate drift over time (e.g., 00:01.016, 00:02.032, 00:03.048).
			/// Each execution is delayed by the interval plus any processing time.
			/// </summary>
			Relative
		};

		template<class TBody>
#if __cpp_lib_move_only_function
		using function_t = std::move_only_function<TBody>;
#else
		using function_t = std::function<TBody>;
#endif

	private:
		class Task
		{
		public:
			virtual void Update(TimerManager* tm) = 0;
			virtual void Throw() = 0;
		};

		class Interval : public Task
		{
		private:
			function_t<void()> m_Func;
			const TimeSpan m_Interval;
			const IntervalMode m_Mode;
			TimeSpan m_LastTime = {};
			std::exception_ptr m_ExceptionPtr;

		public:
			Interval(function_t<void()> func, const TimeSpan& interval, IntervalMode mode);

			virtual void Update(TimerManager* tm) override;
			virtual void Throw() override;
		};

	private:
		PerformanceTimer m_Timer;
		TimeSpan m_TimeSinceStartup;
		TimeSpan m_DeltaTime;

		std::mutex m_Mutex;
		std::unordered_map<int64, std::shared_ptr<Task>> m_Tasks;
		std::vector<std::shared_ptr<Task>> m_Buffer;
		int64 m_TaskId = 0;

	public:
		void Start();
		void StartFrame();
		void UpdateTasks();

		inline const TimeSpan& GetTimeSinceStartup() const { return m_TimeSinceStartup; }
		inline const TimeSpan& GetDeltaTime() const { return m_DeltaTime; }

		int64 AddInterval(function_t<void()> func, const TimeSpan& interval, IntervalMode mode = IntervalMode::Absolute);
		void RemoveTask(int64 taskId);
	};
}