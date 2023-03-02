// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Launch.h"
#include "Logging/Log.h"
#include "GenericPlatform/GenericSplash.h"
#include "Bootstrap/BootstrapTask.h"
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

    NBootstrapTask LoadingTask(TEXT("Test"), 100);
    for (int32 i = 0; i < 100; ++i)
    {
        LoadingTask.Step(1);
        std::this_thread::sleep_for(100ms);
    }

    // Finished the initialization.
    NGenericSplash::Hide();

    // Shutting down.
	Log::FlushAll();

    return Loop->GetExitCode();
}

NLaunch& NLaunch::Get() noexcept
{
    return *CurrentLaunch;
}