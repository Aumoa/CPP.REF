// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Launch.h"
#include "EngineLoop.h"
#include "GameEngine.h"
#include "GenericPlatform/GenericSplash.h"
#include "GenericPlatform/GenericApplication.h"

#if WITH_EDITOR
#include "CoreEd.h"
#endif

#include "Localizational/Name.h"

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

    Loop->PreInit(CmdArgs);
#if WITH_EDITOR
    NCoreEd::EditorInit(*Loop);
#else
    Loop->Init(AGameEngine::StaticClass());
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