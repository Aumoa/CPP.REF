// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Launch.h"
#include "Logging/Log.h"
#include "EngineLoop.h"

NLaunch::NLaunch(String CmdArgs)
	: CmdArgs(CmdArgs)
{
    Loop = std::make_unique<NEngineLoop>();
}

NLaunch::~NLaunch() noexcept
{
}

int32 NLaunch::GuardedMain()
{
    // Ready subresources for initialize engine.
    auto InitContext = Loop->PreInit();

    // Shutting down.
	Log::FlushAll();

    return Loop->GetExitCode();
}