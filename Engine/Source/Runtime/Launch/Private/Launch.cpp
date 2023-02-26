// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Launch.h"
#include "Logging/Log.h"

NLaunch::NLaunch(String CmdArgs)
	: CmdArgs(CmdArgs)
{
}

NLaunch::~NLaunch() noexcept
{
}

int32 NLaunch::GuardedMain(std::function<int32()> Main)
{
	int32 ReturnValue = Main();
	Log::FlushAll();
	return ReturnValue;
}