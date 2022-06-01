// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Core.h"
#include "Threading/Tasks/Task.h"
#include "Logging/Log.h"

void Core::Initialize()
{
	Task<>::_Initialize();
	Log::_Initialize();
}

void Core::Shutdown()
{
	Log::_Shutdown();
	Task<>::_Shutdown();
}