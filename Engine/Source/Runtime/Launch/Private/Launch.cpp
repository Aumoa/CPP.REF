// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Launch.h"
#include "EditorEngineLoop.h"
#include "GenericPlatform/GenericSplash.h"
#include "GenericPlatform/GenericApplication.h"

NLaunch* NLaunch::CurrentLaunch;

NLaunch::NLaunch(String CmdArgs)
	: CmdArgs(CmdArgs)
{
    check(CurrentLaunch == nullptr);
    CurrentLaunch = this;

    GenericApp = NGenericApplication::CreateApplication();
    Loop = std::make_unique<NEditorEngineLoop>();
}

NLaunch::~NLaunch() noexcept
{
    check(CurrentLaunch);
    CurrentLaunch = nullptr;
}

int32 NLaunch::GuardedMain()
{
    GenericApp->SetApplicationPointer(GetApplicationPointer());

    // Ready subresources for initialize engine.
    auto InitContext = Loop->PreInit();

    // Initialize engine. 
    Loop->Init(InitContext.get());

    // Finished the initialization.
    Loop->PostInit(std::move(InitContext));

    while (!GenericApp->IsQuitRequested())
    {
        GenericApp->PumpMessages();
        Loop->Tick();
    }

    return GenericApp->GetExitCode();
}

NLaunch& NLaunch::Get() noexcept
{
    return *CurrentLaunch;
}