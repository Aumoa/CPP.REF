// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Launch.h"

NLaunch::NLaunch(String CmdArgs)
	: CmdArgs(CmdArgs)
{
}

NLaunch::~NLaunch() noexcept
{
}

int32 NLaunch::GuardedMain()
{
	return 0;
}