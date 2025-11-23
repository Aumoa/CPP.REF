// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Threading/SynchronizationContext.h"

namespace Ayla
{
	thread_local SynchronizationContext* g_Current;

	void SynchronizationContext::SetSynchronizationContext(SynchronizationContext* context)
	{
		g_Current = context;
	}

	SynchronizationContext* SynchronizationContext::GetCurrent()
	{
		return g_Current;
	}
}