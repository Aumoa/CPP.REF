// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Diagnostics/PerformanceTimer.h"

namespace Ayla
{
	class ENGINE_API TimerManager
	{
	private:
		PerformanceTimer m_Timer;
		TimeSpan m_TimeSinceStartup;
		TimeSpan m_DeltaTime;

	public:
		void Start();
		void StartFrame();

		inline const TimeSpan& GetTimeSinceStartup() const { return m_TimeSinceStartup; }
		inline const TimeSpan& GetDeltaTime() const { return m_DeltaTime; }
	};
}