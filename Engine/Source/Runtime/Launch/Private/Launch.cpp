// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Launch.h"
#include "EngineLoop.h"
#include "GameEngine.h"
#include "Global.h"
#include "GenericPlatform/GenericSplash.h"
#include "GenericPlatform/GenericApplication.h"
#include "Localizational/Name.h"

#if WITH_EDITOR
#include "CoreEd.h"
#endif

NLaunch* NLaunch::CurrentLaunch;

NLaunch::NLaunch(String CmdArgs)
	: CmdArgs(CmdArgs)
{
    check(CurrentLaunch == nullptr);
    CurrentLaunch = this;

    GenericApp = NGenericApplication::CreateApplication();
    Loop = std::make_unique<NEngineLoop>();
}

NLaunch::~NLaunch() noexcept
{
    check(CurrentLaunch);
    CurrentLaunch = nullptr;
}

int32 NLaunch::GuardedMain()
{
    GenericApp->SetApplicationPointer(GetApplicationPointer());
    Global::SetEngineDirectory(GenericApp->GetEngineDirectory());
    Global::SetGameDirectory(Environment::GetCurrentDirectory());

    Loop->PreInit(CmdArgs);
#if WITH_EDITOR
    NCoreEd::EditorInit(*Loop);
#else
    Loop->Init(std::make_shared<GameEngine>());
#endif
    Loop->PostInit();

    std::vector<NGenericPlatformInputEvent> InputEvents;
    while (!GenericApp->IsQuitRequested())
    {
        GenericApp->PumpMessages(InputEvents);
        Loop->Tick(InputEvents);
    }

    Loop->Shutdown();
    return GenericApp->GetExitCode();
}

NLaunch& NLaunch::Get() noexcept
{
    return *CurrentLaunch;
}