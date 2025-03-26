// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "TimeSpan.h"
#include "DateTime.h"
#include <optional>

namespace Ayla
{
	class CORE_API PerformanceTimer
	{
	public:
		PerformanceTimer() noexcept {}
		PerformanceTimer(std::in_place_t) noexcept { Start(); }
		PerformanceTimer(const PerformanceTimer& Rhs) noexcept;
		PerformanceTimer(PerformanceTimer&& Rhs) noexcept;
		~PerformanceTimer() noexcept {}

		void Start() noexcept;
		void Stop() noexcept;

		void Reset() noexcept;
		void Restart() noexcept;

		bool IsRunning() const noexcept;
		TimeSpan GetElapsed() const noexcept;

		static PerformanceTimer StartNew() noexcept;

	public:
		PerformanceTimer& operator =(const PerformanceTimer& Rhs) noexcept;
		PerformanceTimer& operator =(PerformanceTimer&& Rhs) noexcept;

	private:
		std::optional<DateTime> StartTime;
		TimeSpan Elapsed;
	};
}