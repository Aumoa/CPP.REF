// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Core.h"
#include "GC.h"
#include "Threading/Tasks/Task.h"
#include "Logging/Log.h"

void Core::Initialize()
{
	Task<>::Initialize();
	Log::Initialize();
	GC::Initialize();
}

void Core::Shutdown()
{
	GC::Shutdown();
	Log::Shutdown();
	Task<>::Shutdown();
}