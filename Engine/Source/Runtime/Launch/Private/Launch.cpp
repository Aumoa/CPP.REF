// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Launch.h"
#include "Logging/Log.h"
#include "GenericPlatform/GenericSplash.h"
#include "EngineLoop.h"

NLaunch* NLaunch::CurrentLaunch;

NLaunch::NLaunch(String CmdArgs)
	: CmdArgs(CmdArgs)
{
    check(CurrentLaunch == nullptr);
    CurrentLaunch = this;
    Loop = std::make_unique<NEngineLoop>();
}

NLaunch::~NLaunch() noexcept
{
    check(CurrentLaunch);
    CurrentLaunch = nullptr;
}

int32 NLaunch::GuardedMain()
{
    // Ready subresources for initialize engine.
    auto InitContext = Loop->PreInit();

    // Initialize engine. 
    Loop->Init();

    // Finished the initialization.
    NGenericSplash::Hide();
    NBootstrapTask::Clear();

    // Shutting down.
	Log::FlushAll();

    return Loop->GetExitCode();
}

NLaunch& NLaunch::Get() noexcept
{
    return *CurrentLaunch;
}