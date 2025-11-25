// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "TimerManager.h"

namespace Ayla
{
	void TimerManager::Start()
	{
		m_Timer.Start();
		m_TimeSinceStartup = {};
		m_DeltaTime = {};
	}

	void TimerManager::StartFrame()
	{
		auto elapsed = m_Timer.GetElapsed();
		m_DeltaTime = elapsed - m_TimeSinceStartup;
		m_TimeSinceStartup = elapsed;
	}
}